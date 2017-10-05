#sender

import socket
import binascii
import struct
import random
import threading
import time
from multiprocessing import Process

flag = 1
seq = 0
time_flag = 0
time_out = 0
err = 0

host = '127.0.0.1'
port = 9189

sender = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sender.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sender.connect((host, port))

lock = threading.Lock()

def timer():
    global time_out
    global time_flag
    if time_flag == 0:
        for i in range(5):
            if time_flag == 1:
                time_flag = 0
                return
            time.sleep(1)
        lock.acquire()
        time_out = 1
        time_flag = 0
        lock.release()
        return
    else:
        return

def SEND():
    global flag
    global seq
    global time_flag
    global time_out
    global err
    try:
        while True:
            data_type = random.random()
            lock.acquire()
            seq = (seq + 1) % 2
            lock.release()
            if data_type < 0.3:
                random_str = str(data_type)
                crc = hex(binascii.crc32(random_str) & 0xffffffff)
                data = str(seq) + random_str + crc
                data = struct.pack('1024s', data)
                #copy = data
            if 0.3 < data_type < 0.6:
                data = 'lose'
                copy = struct.pack('1024s', str(seq) + 'resend_data')
            if 0.6 < data_type < 1:
                data = 'err'
                copy = struct.pack('1024s', str(seq) + 'resend_data')
            #lock.acquire()
            #seq = (seq + 1) % 2
            #lock.release()
            #print data
            
            if flag == 1:
                lock.acquire()
                sender.send(data)
                print 'send:', data
                #lock.acquire()
                flag = 0
                print 'seq_flag:', seq
                #if data == 'lose':
                #    lock.acquire()
                #    flag = 1
                #    lock.release()
                #seq = (seq + 1) % 2
                lock.release()
            timer() 
#            print "jishijieshu"
            if time_out == 1 or err == 1:
                lock.acquire()
                print 'chongfa'
                sender.send(copy)
                #print 'data:', copy
                #lock.acquire()
                time_out = 0
                err = 0
                seq = (seq + 1) % 2
                print 'seq_time_out:', seq
                lock.release()

    except KeyboardInterrupt:
        exit(0)

def RECV():
    global flag
    global seq
    global time_flag
    global err
    try:
        while True:
            ack = sender.recv(1024)
            
            print 'ack:', ack
            print 'seq:', seq
            if str((seq + 1) % 2) == str(ack):
                lock.acquire()
                flag = 1
                time_flag = 1
                lock.release()
                print 'ack:', ack
            if str(ack) == 'nak' or str((seq + 1) % 2) != str(ack):
                print 'nak'
                lock.acquire()
                err = 1
                time_flag = 1
                lock.release()

    except KeyboardInterrupt:
        exit(0)
def p():
    send_thread = threading.Thread(target = SEND)
    send_thread.start()
    recv_thread = threading.Thread(target = RECV)
    recv_thread.start()
    while True:
        pass

proc = Process(target = p)
proc.start()

