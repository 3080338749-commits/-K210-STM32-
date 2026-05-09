import sensor
import image
import lcd
import KPU as kpu
import time
from Maix import FPIOA, GPIO
import gc
from fpioa_manager import fm
from board import board_info
import utime
from machine import UART
from machine import Timer

#变量定义
Serial_A_RxBuf = bytearray()

#映射UART2的两个引脚
fm.register(GPIO.GPIOHS9,fm.fpioa.UART2_TX)
fm.register(GPIO.GPIOHS10,fm.fpioa.UART2_RX)

task_fd = kpu.load(0x100000)
task_ld = kpu.load(0x200000)
task_fe = kpu.load(0x300000)

#task_fd = kpu.load("/sd/FaceDetection.smodel")
#task_ld = kpu.load("/sd/FaceLandmarkDetection.smodel")
#task_fe = kpu.load("/sd/FeatureExtraction.smodel")

#初始化串口，返回调用句柄
uart_A = UART(UART.UART2, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)

clock = time.clock()

fm.register(board_info.BOOT_KEY, fm.fpioa.GPIOHS0)
key_gpio = GPIO(GPIO.GPIOHS0, GPIO.IN)
start_processing = False            #人脸录入标志

BOUNCE_PROTECTION = 50

def set_key_state(*_):
    global start_processing
    start_processing = True
    utime.sleep_ms(BOUNCE_PROTECTION)

def func_serial2(timer):
    #while True:
    #print("hello {}".format(name))
    uart_A.write("hello serial2")
    #time.sleep(1)


def serial2_cmd_send(cmd, val):
    cmd_buf = bytearray([0xAF, 0x01, 0x02, 0xFA])

    cmd_buf[1] = cmd
    cmd_buf[2] = val
    uart_A.write(cmd_buf)

def face_del_all():     #人脸清除
    record_ftrs.clear()

#key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)      # 按键录入人脸

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1)
#sensor.set_vflip(1)
sensor.run(1)
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437,
          6.92275, 6.718375, 9.01025)  # anchor for face detect
dst_point = [(44, 59), (84, 59), (64, 82), (47, 105),
             (81, 105)]  # standard face key point position
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor)
img_lcd = image.Image()
img_face = image.Image(size=(128, 128))
a = img_face.pix_to_ai()
record_ftr = []
record_ftrs = []
names = ['Mr.1', 'Mr.2', 'Mr.3', 'Mr.4', 'Mr.5',
         'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9', 'Mr.10']

ACCURACY = 70       #相似度

#tim = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PERIODIC, period=1000, callback=func_serial2, arg=func_serial2)       #串口任务


while (1):
    img = sensor.snapshot()
    clock.tick()
    code = kpu.run_yolo2(task_fd, img)
    if code:
        for i in code:
            # Cut face and resize to 128x128
            a = img.draw_rectangle(i.rect())
            face_cut = img.cut(i.x(), i.y(), i.w(), i.h())
            face_cut_128 = face_cut.resize(128, 128)
            a = face_cut_128.pix_to_ai()
            # a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points
            fmap = kpu.forward(task_ld, face_cut_128)
            plist = fmap[:]
            le = (i.x() + int(plist[0] * i.w() - 10), i.y() + int(plist[1] * i.h()))
            re = (i.x() + int(plist[2] * i.w()), i.y() + int(plist[3] * i.h()))
            nose = (i.x() + int(plist[4] * i.w()), i.y() + int(plist[5] * i.h()))
            lm = (i.x() + int(plist[6] * i.w()), i.y() + int(plist[7] * i.h()))
            rm = (i.x() + int(plist[8] * i.w()), i.y() + int(plist[9] * i.h()))
            a = img.draw_circle(le[0], le[1], 4)
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4)
            # align face to standard position
            src_point = [le, re, nose, lm, rm]
            T = image.get_affine_transform(src_point, dst_point)
            a = image.warp_affine_ai(img, img_face, T)
            a = img_face.ai_to_pix()
            # a = img.draw_image(img_face, (128,0))
            del (face_cut_128)
            # calculate face feature vector
            fmap = kpu.forward(task_fe, img_face)
            feature = kpu.face_encode(fmap[:])
            reg_flag = False
            scores = []
            for j in range(len(record_ftrs)):
                score = kpu.face_compare(record_ftrs[j], feature)
                scores.append(score)
            max_score = 0
            index = 0
            for k in range(len(scores)):
                if max_score < scores[k]:
                    max_score = scores[k]
                    index = k                   #找到对应的人脸序号
            if max_score > ACCURACY:                #特征值满足 识别成功
                a = img.draw_string(i.x(), i.y(), ("%s :%2.1f" % (
                    names[index], max_score)), color=(0, 255, 0), scale=2)
                serial2_cmd_send(2, index)
            else:
                a = img.draw_string(i.x(), i.y(), ("X :%2.1f" % (
                    max_score)), color=(255, 0, 0), scale=2)
            if start_processing:                    #人脸录入
                record_ftr = feature
                print("record_ftr: ")
                #print(record_ftr)
                record_ftrs.append(record_ftr)      #将当前特征添加到已知特征列表

                print("录入完成")
                serial2_cmd_send(5, 0)
                start_processing = False            #录入完成
            break
    fps = clock.fps()
    #print("%2.1f fps" % fps)
    #img.rotation_corr(x_rotation=180,y_rotation=180)
    a = lcd.display(img)
    gc.collect()
    # kpu.memtest()

    #串口任务
    if(uart_A.any()):
        temp = uart_A.read(5)
        if(temp):
            if(bytes([temp[0]]) == b'\xa1') and (bytes([temp[3]]) == b'\x1a'):         # 比较数据相等
                print("---")
                if(bytes([temp[1]]) == b'\x01') and (bytes([temp[2]]) == b'\x02'):     # 人脸删除
                    print("Face Del")
                    face_del_all()
                    serial2_cmd_send(1, 0)
                elif(bytes([temp[1]]) == b'\x02') and (bytes([temp[2]]) == b'\x00'):   #人脸录入
                    print("Face Add")
                    start_processing
                    start_processing = True


# a = kpu.deinit(task_fe)
# a = kpu.deinit(task_ld)
# a = kpu.deinit(task_fd)

