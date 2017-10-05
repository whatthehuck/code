#receiver

import socket
import binascii
import struct
import threading
from multiprocessing import Process

seq = 0
err = 0
lose = 0
normal = 0

host = "127.0.0.1"
port = 9189
receiver = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
receiver.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

receiver.bind((host, port))

receiver.listen(1)

conn, addr = receiver.accept()

lock = threading.Lock()

def RECEIVE():
    global seq
    global err
    global lose
    global normal
    try:
        while True:
            packed_data = conn.recv(1024)
            #data, = struct.unpack('1024s', packed_data)
            print 'recv:', packed_data
            if packed_data == 'lose':
                lock.acquire()
                lose = 1
                lock.release()
                continue
            if packed_data == 'err':
                lock.acquire()
                err = 1
                lock.release()
                continue
            else:
                print "normal"
                data, = struct.unpack('1024s', packed_data)
                lock.acquire()
                seq = int(data[0])
                normal = 1
                lose = 0
                err = 0
                lock.release()
                print 'normal:', normal
            
            #print seq
    except KeyboardInterrupt:
        receiver.close()
        exit(0)

def SEND():
    global seq
    global lose
    global err
    global normal
    try:
        while True:
            if lose == 1:
                continue
            if err == 1:
                lock.acquire()
                err = 0
                lock.release()
                conn.send('nak')
            if normal == 1:
                lock.acquire()
                lose = 0
                err = 0
                normal = 0
                seq = (seq + 1) % 2
                conn.send(str(seq))
                lock.release()
                print 'send:', seq
    except KeyboardInterrupt:
        receiver.close()
        exit(0)

def p():
    recv_thread = threading.Thread(target = RECEIVE)
    recv_thread.start()
    send_thread = threading.Thread(target = SEND)
    send_thread.start()
    while True:
        pass

proc = Process(target = p)
proc.start()

