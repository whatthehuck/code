import socket
from threading import *
from Tkinter import *
import os
import struct
from ctypes import *
from scapy.all import *

all_package = NONE

arp_package_content = NONE
rarp_package_content = NONE

#icmp varible
icmp_raw_data = NONE
icmp_package = NONE
icmp_version = NONE
icmp_service_field = NONE
icmp_len = NONE
icmp_id = NONE
icmp_flags = NONE
icmp_offset = NONE
icmp_ttl = NONE
icmp_protocol_num = NONE
icmp_checksum = NONE
icmp_src = NONE
icmp_dst = NONE

#tcp varible
tcp_raw_data = NONE
tcp_package = NONE
tcp_version = NONE
tcp_service_field = NONE
tcp_total_length = NONE
tcp_id = NONE
tcp_flags = NONE
tcp_offset = NONE
tcp_ttl = NONE
tcp_protocol = NONE
tcp_ch = NONE
tcp_src = NONE
tcp_dst = NONE
tcp_src_port = NONE
tcp_dst_port = NONE
tcp_seq = NONE
tcp_ack = NONE
tcp_h_len = NONE
tcp_flag = NONE
tcp_winsize = NONE
tcp_checksum = NONE
tcp_u_point = NONE

#udp varible
udp_raw_data = NONE
udp_package = NONE
udp_version = NONE
udp_service_field = NONE
udp_total_length = NONE
udp_id = NONE
udp_flags = NONE
udp_offset = NONE
udp_ttl = NONE
udp_protocol = NONE
udp_h_checksum = NONE
udp_src = NONE
udp_dst = NONE
udp_src_port = NONE
udp_dst_port = NONE
udp_length = NONE
udp_checksum = NONE

#igmp varible
igmp_raw_data = NONE
igmp_package = NONE
igmp_version = NONE
igmp_service_field = NONE
igmp_total_length = NONE
igmp_id = NONE
igmp_flags = NONE
igmp_offset = NONE
igmp_ttl = NONE
igmp_protocol = NONE
igmp_h_checksum = NONE
igmp_src = NONE
igmp_dst = NONE
igmp_options = NONE


host = "192.168.1.109"

root = Tk()

def reverse(hex_str):
    #print 'hex:', hex_str
    hex_str = hex(hex_str)
    #print hex_str
    l = []
    temp = ''
    for i in range(0, len(hex_str), 2):
        l.append(hex_str[i:i + 2])
    
    for j in range(len(l)):
        temp = temp + l[-j]

    return temp
#---------------------------------------------------------------
class ICMP(Structure):
    _fields_ = [
        ("version", c_ubyte),
        ("service_field", c_ubyte),
        ("len", c_ushort),
        ("id", c_ushort),
        ("flags", c_ubyte),
        ("offset", c_ubyte),
        ("ttl", c_ubyte),
        ("protocol_num", c_ubyte, 2),
        ("checksum", c_ushort),
        ("src", c_uint, 32),
        ("dst", c_uint, 32)
    ]
    def __new__(self, socket_buffer = None):
        return_buffer = self.from_buffer_copy(socket_buffer)
        return return_buffer
    def __init__(self, socket_buffer = None):
        self.protocol_map = {1:"ICMP", 6:"TCP", 17:"UDP"}
        self.version_r = reverse(self.version)
        self.service_field_r = reverse(self.service_field)
        self.len_r = reverse(self.len)
        self.id_r = reverse(self.id)
        self.flags_r = reverse(self.flags)
        self.offset_r = reverse(self.offset)
        self.ttl_r = reverse(self.ttl)
        self.protocol_num_r = reverse(self.protocol_num)
        self.checksum_r = reverse(self.checksum)
        
        self.src_address = socket.inet_ntoa(struct.pack("L", self.src)[0:4])
        self.dst_address = socket.inet_ntoa(struct.pack("L", self.dst)[0:4])
        
        try:
            self.protocol = self.protocol_map[self.protocol_num]
        except:
            self.protocol = str(self.protocol_num)
#---------------------------------------------------------------

class TCP(Structure):
    _fields_ = [
        ('version', c_ubyte),
        ('service_field', c_ubyte),
        ('total_length', c_ushort),
        ('id', c_ushort),
        ('flags', c_ubyte),
        ('offset', c_ubyte),
        ('ttl', c_ubyte),
        ('protocol', c_ubyte),
        ('ch', c_ushort),
        ('src', c_uint, 32),
        ('dst', c_uint, 32),
        ('src_port', c_ushort),
        ('dst_port', c_ushort),
        ('seq', c_uint, 32),
        ('ack', c_uint, 32),
        ('h_len', c_ubyte),
        ('flag', c_ushort),
        ('winsize', c_ushort),
        ('checksum', c_ushort),
        ('u_point', c_ushort)
    ]

    def __new__(self, socket_buffer = None):
        return self.from_buffer_copy(socket_buffer)
    def __init__(self, socket_buffer):

        ip_src = socket.inet_ntoa(struct.pack('L', self.src)[0:4])
        ip_dst = socket.inet_ntoa(struct.pack('L', self.dst)[0:4])

        self.version_r = reverse(self.version)
        self.service_field_r = reverse(self.service_field)
        self.total_length_r = reverse(self.total_length)
        self.id_r = reverse(self.id)
        self.flags_r = reverse(self.flags)
        self.offset_r = reverse(self.offset)
        self.ttl_r = reverse(self.ttl)
        self.protocol_r = reverse(self.protocol)
        self.ch_r = reverse(self.ch)
        self.src_address = ip_src
        self.dst_address = ip_dst
        self.src_port_r = reverse(self.src_port)
        self.dst_port_r = reverse(self.dst_port)
        self.seqnum_r = reverse(self.seq)
        self.acknum_r = reverse(self.ack)
        self.len_r = reverse(self.h_len)
        self.flag_r = reverse(self.flag)
        self.winsize_r = reverse(self.winsize)
        self.checksum_r = reverse(self.checksum)
        self.u_point_r = reverse(self.u_point)
#----------------------------END-------------------------------
#----------------------------UDP-------------------------------
class UDP(Structure):
    _fields_ = [
        ('version', c_ubyte),
        ('service_field', c_ubyte),
        ('total_length', c_ushort),
        ('id', c_ushort),
        ('flags', c_ubyte),
        ('offset', c_ubyte),
        ('ttl', c_ubyte),
        ('protocol', c_ubyte),
        ('h_checksum', c_ushort),
        ('src', c_uint, 32),
        ('dst', c_uint, 32),
        ('src_port', c_ushort),
        ('dst_port', c_ushort),
        ('length', c_ushort),
        ('checksum', c_ushort)
    ]
    
    def __new__(self, socket_buffer = None):
        return self.from_buffer_copy(socket_buffer)
    def __init__(self, socket_buffer = None):
        src_ip = socket.inet_ntoa(struct.pack('L', self.src)[0:4])
        dst_ip = socket.inet_ntoa(struct.pack('L', self.dst)[0:4])
        self.version_r = reverse(self.version)
        self.service_field_r = reverse(self.service_field)
        self.total_length_r = reverse(self.total_length)
        self.id_r = reverse(self.id)
        self.flags_r = reverse(self.flags)
        self.offset_r = reverse(self.offset)
        self.ttl_r = reverse(self.ttl)
        self.protocol_r = reverse(self.protocol)
        self.h_checksum_r = reverse(self.h_checksum)
        self.src_r = src_ip
        self.dst_r = dst_ip
        self.src_port_r = reverse(self.src_port)
        self.dst_port_r = reverse(self.dst_port)
        self.length_r = reverse(self.length)
        self.checksum_r = reverse(self.checksum)
#------------------------END-----------------------------------

#------------------------IGMP----------------------------------
class IGMP(Structure):
    _fields_ = [
        ('version', c_ubyte),
        ('service_field', c_ubyte),
        ('total_length', c_ushort),
        ('id', c_ushort),
        ('flags', c_ubyte),
        ('offset', c_ubyte),
        ('ttl', c_ubyte),
        ('protocol', c_ubyte),
        ('h_checksum', c_ushort),
        ('src', c_uint, 32),
        ('dst', c_uint, 32),
        ('options', c_uint, 32)
    ]

    def __new__(self, socket_buffer = None):
        return self.from_buffer_copy(socket_buffer)
    def __init__(self, socket_buffer = None):
        src_ip = socket.inet_ntoa(struct.pack('L', self.src)[0:4])
        dst_ip = socket.inet_ntoa(struct.pack('L', self.dst)[0:4])
        
        self.version_r = reverse(self.version)
        self.service_field_r = reverse(self.service_field)
        self.total_length_r = reverse(self.total_length)
        self.id_r = reverse(self.id)
        self.flags_r = reverse(self.flags)
        self.offset_r = reverse(self.offset)
        self.ttl_r = reverse(self.ttl)
        self.protocol_r = reverse(self.protocol)
        self.h_checksum_r = reverse(self.h_checksum)
        self.src_r = src_ip
        self.dst_r = dst_ip
        self.options_r = reverse(self.options)
#------------------------END-----------------------------------

#------------------------ICMP----------------------------------
def icmp():
    global icmp_version, icmp_service_field, icmp_len
    global icmp_id, icmp_flags
    global icmp_offset, icmp_ttl, icmp_protocol_num
    global icmp_checksum
    global icmp_src
    global icmp_dst
    global icmp_package, icmp_raw_data
    global all_package
    if os.name == "nt":
        socket_protocol = socket.IPPROTO_IP
    else:
        socket_protocol = socket.IPPROTO_ICMP

    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket_protocol)

    #sniffer.bind((host, 0))
    #sniffer.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

    if os.name == "nt":
        sniffer.ioctl(socket.SIO_RCVALL, socket.RCVALL_ON)

    try:
        while True:
            raw_buffer = sniffer.recvfrom(65565)[0]
            icmp_raw_data = "raw data: " + raw_buffer.encode("hex") + "\n"
            ip_header = ICMP(raw_buffer[0:20])

            icmp_version = "version: " + ip_header.version_r + "\n"
            icmp_service_field = "service_field: " + ip_header.service_field_r + "\n"
            icmp_len = "len: " + ip_header.len_r + "\n"
            icmp_id = "id: " + ip_header.id_r + "\n"
            icmp_flags = "flags: " + ip_header.flags_r + "\n"
            icmp_offset = "offset: " + ip_header.offset_r + "\n"
            icmp_ttl = "ttl: " + ip_header.ttl_r + "\n"
            icmp_protocol_num = "protocol_num: " + ip_header.protocol_num_r + "\n"
            icmp_checksum = "checksum: " + ip_header.checksum_r + "\n"
            icmp_src = "src: " + ip_header.src_address + "\n"
            icmp_dst = "dst: " + ip_header.dst_address + "\n"

            icmp_package = icmp_raw_data + icmp_version + icmp_service_field \
                            + icmp_len + icmp_id + icmp_flags \
                            + icmp_offset + icmp_ttl + icmp_protocol_num \
                            + icmp_checksum + icmp_src + icmp_dst + "\n"

            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()

            lock.acquire()
            all_package = icmp_package
            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()
            lock.release()


    except KeyboardInterrupt:
        exit(0)
        if os.name == "nt":
            sniffer.ioctl(socket.SIO_RCVALL, socket.RCVALL_OFF)

#-----------------------END-------------------------------------------


#-----------------------TCP-------------------------------------------
def tcp():
    global tcp_raw_data, tcp_package
    global tcp_version, tcp_service_field, tcp_total_length, tcp_id, tcp_flags, tcp_offset
    global tcp_ttl, tcp_protocol, tcp_ch, tcp_src, tcp_dst, tcp_src_port, tcp_dst_port
    global tcp_seq, tcp_ack, tcp_h_len, tcp_flag, tcp_winsize, tcp_checksum , tcp_u_point
    global all_package
    socket_protocol = socket.IPPROTO_TCP
    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket_protocol)
    #sniffer.bind((host, 0))
    #sniffer.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
    try:
        while True:
            raw_buffer = sniffer.recvfrom(65536)[0]
            tcp_raw_data = raw_buffer.encode("hex") + "\n"
            if len(raw_buffer) < 44:
                raw_buffer = raw_buffer + (44 - len(raw_buffer)) * '0'
            tcp_header = TCP(raw_buffer[0:44])

            tcp_version = "version: " + tcp_header.version_r + "\n"
            tcp_service_field = "service_field: " + tcp_header.service_field_r + "\n"
            tcp_total_length = "total length: " + tcp_header.total_length_r + "\n"
            tcp_id = "id: " + tcp_header.id_r + "\n"
            tcp_flags = "flags: " + tcp_header.flags_r + "\n"
            tcp_offset = "offset: " + tcp_header.offset_r + "\n"
            tcp_ttl = "ttl: " + tcp_header.ttl_r + "\n"
            tcp_protocol = "protocol: " + tcp_header.protocol_r + "\n"
            tcp_ch = "ch: " + tcp_header.ch_r + "\n"
            tcp_src = "src: " + tcp_header.src_address + "\n"
            tcp_dst = "dst: " + tcp_header.dst_address + "\n"
            tcp_src_port = "src port: " + tcp_header.src_port_r + "\n"
            tcp_dst_port = "dst port: " + tcp_header.dst_port_r + "\n"
            tcp_seq = "seq: " + tcp_header.seqnum_r + "\n"
            tcp_ack = "ack: " + tcp_header.acknum_r + "\n"
            tcp_h_len = "header length: " + tcp_header.len_r + "\n"
            tcp_flag = "flag: " + tcp_header.flag_r + "\n"
            tcp_winsize = "winsize: " + tcp_header.winsize_r + "\n"
            tcp_checksum = "checksum: " + tcp_header.checksum_r + "\n"
            tcp_u_point = "urgen point: " + tcp_header.u_point_r + "\n"

            tcp_package = tcp_raw_data + tcp_version + tcp_service_field \
                            + tcp_total_length + tcp_id + tcp_flags + tcp_offset \
                            + tcp_ttl + tcp_protocol \
                            + tcp_ch + tcp_src + tcp_dst + tcp_src_port \
                            + tcp_dst_port + tcp_seq + tcp_ack + tcp_h_len \
                            + tcp_flag + tcp_winsize + tcp_checksum \
                            + tcp_u_point + "\n"

            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()

            lock.acquire()
            all_package = tcp_package
            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()
            lock.release()

    except KeyboardInterrupt:
        exit(0)
#---------------------END--------------------------------------

def udp():
    global udp_version, udp_service_field, udp_total_length, udp_id
    global udp_flags, udp_offset, udp_ttl, udp_protocol, udp_h_checksum
    global udp_src, udp_dst, udp_src_port, udp_dst_port, udp_length, udp_checksum
    global udp_package, udp_raw_data
    global all_package
    socket_protocol = socket.IPPROTO_UDP
    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket_protocol)
    try:
        while True:
            raw_buffer = sniffer.recvfrom(65536)[0]
            udp_raw_data = raw_buffer.encode('hex') + "\n"
            udp_header = UDP(raw_buffer)

            udp_version = "version: " + udp_header.version_r + "\n"
            udp_service_field = "service_field: " + udp_header.service_field_r + "\n"
            udp_total_length = "total length: " + udp_header.total_length_r + "\n"
            udp_id = "id: " + udp_header.id_r + "\n"
            udp_flags = "flags: " + udp_header.flags_r + "\n"
            udp_offset = "offset: " + udp_header.offset_r + "\n"
            udp_ttl = "ttl: " + udp_header.ttl_r + "\n"
            udp_protocol = "protocol: " + udp_header.protocol_r + "\n"
            udp_h_checksum = "ch: " + udp_header.h_checksum_r + "\n"
            udp_src = "src: " + udp_header.src_r + "\n"
            udp_dst = "dst: " + udp_header.dst_r + "\n"
            udp_src_port = "src port: " + udp_header.src_port_r + "\n"
            udp_dst_port = "dst port: " + udp_header.dst_port_r + "\n"
            udp_length = "header length: " + udp_header.length_r + "\n"
            udp_checksum = "checksum: " + udp_header.checksum_r + "\n"

            udp_package = udp_version + udp_service_field + udp_total_length \
                          + udp_id + udp_flags + udp_offset + udp_ttl \
                          + udp_protocol + udp_h_checksum \
                          + udp_src + udp_dst + udp_src_port + udp_dst_port \
                          + udp_length + udp_checksum + "\n"

            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()

            lock.acquire()
            all_package = udp_package
            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()
            lock.release()

    except KeyboardInterrupt:
        exit(0)
#--------------------------------------------------------------

def igmp():
    global igmp_raw_data, igmp_package
    global igmp_version, igmp_service_field, igmp_total_length, igmp_id
    global igmp_flags, igmp_offset, igmp_ttl, igmp_protocol
    global igmp_h_checksum, igmp_src, igmp_dst, igmp_options
    global all_package
    socket_protocol = socket.IPPROTO_IGMP
    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket_protocol)
    try:
        while True:
            raw_buffer = sniffer.recvfrom(65536)[0]
            igmp_raw_data = raw_buffer.encode('hex') + "\n"
            igmp_header = IGMP(raw_buffer)

            igmp_version = "version: " + igmp_header.version_r + "\n"
            igmp_service_field = "service_field: " + igmp_header.service_field_r + "\n"
            igmp_total_length = "total length: " + igmp_header.total_length_r + "\n"
            igmp_id = "id: " + igmp_header.id_r + "\n"
            igmp_flags = "flags: " + igmp_header.flags_r + "\n"
            igmp_offset = "offset: " + igmp_header.offset_r + "\n"
            igmp_ttl = "ttl: " + igmp_header.ttl_r + "\n"
            igmp_protocol = "protocol_num: " + igmp_header.protocol_r + "\n"
            igmp_h_checksum = "checksum: " + igmp_header.h_checksum_r + "\n"
            igmp_src = "src: " + igmp_header.src_r + "\n"
            igmp_dst = "dst: " + igmp_header.dst_r + "\n"

            igmp_package = igmp_raw_data + igmp_version + igmp_service_field \
                   + igmp_total_length + igmp_id + igmp_flags \
                   + igmp_offset + igmp_ttl + igmp_protocol \
                   + igmp_h_checksum + igmp_src + igmp_dst + "\n"

            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()

            lock.acquire()
            all_package = igmp_package
            root.event_generate("<<COMRxRdy>>", when="tail")
            root.update()
            lock.release()

    except KeyboardInterrupt:
        exit(0)
#------------------------------END-----------------------------
'''
def arp_content(x):
    global arp_package_content
    global all_package
    arp_package_content = hexdump(x)
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()

    lock.acquire()
    all_package = arp_package_content
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()
    lock.release()

def rarp_content(x):
    global rarp_package_content
    global all_package
    rarp_package_content = hexdump(x)
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()

    lock.acquire()
    all_package = rarp_package_content
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()
    lock.release()
'''
#----------------------------ARP-------------------------------
def arp():
    global arp_package_content
    global all_package
    #sniff(iface = "wlp4s0", filter = "arp", prn = arp_content)
    arp_package_content = str(sniff(iface = "wlp4s0", filter = "arp", count = 1))
    arp_package_content += "\n"
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()

    lock.acquire()
    all_package = arp_package_content
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()
    lock.release()

#-----------------------------END------------------------------

#----------------------------RARP------------------------------
def rarp():
    global arp_package_content
    global all_package
    #sniff(iface = "wlp4s0", filter = "arp", prn = arp_content)
    rarp_package_content = str(sniff(iface = "wlp4s0", filter = "rarp", count = 1))
    rarp_package_content += "\n"
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()

    lock.acquire()
    all_package = rarp_package_content
    root.event_generate("<<COMRxRdy>>", when="tail")
    root.update()
    lock.release()


#----------------------------END-----------------------------

'''
process_tcp = NONE
process_icmp = NONE
process_udp = NONE
process_igmp = NONE

def printkey(event):
    global process_tcp, process_icmp, process_udp, process_igmp
    if event.char == '1':
        if process_icmp != NONE:
            if process_icmp.is_alive() == True:
                process_icmp.terminate()
        if process_udp != NONE:
            if process_udp.is_alive() == True:
                process_udp.terminate()
        if process_igmp != NONE:
            if process_igmp.is_alive() == True:
                process_igmp.terminate()
        process_tcp = Process(target = tcp)
        process_tcp.start()
    if event.char == '2':
        if process_tcp != NONE:
            if process_tcp.is_alive() == True:
                process_tcp.terminate()
        if process_udp != NONE:
            if process_udp.is_alive() == True:
                process_udp.terminate()
        if process_igmp != NONE:
            if process_igmp.is_alive() == True:
                process_igmp.terminate()
        process_icmp = Process(target = icmp)
        process_icmp.start()
    if event.char == '3':
        if process_tcp != NONE:
            if process_tcp.is_alive() == True:
                process_tcp.terminate()
        if process_icmp != NONE:
            if process_icmp.is_alive() == True:
                process_icmp.terminate()
        if process_igmp != NONE:
            if process_igmp.is_alive() == True:
                process_igmp.terminate()
        process_udp = Process(target = udp)
        process_udp.start()
    if event.char == '4':
        if process_tcp != NONE:
            if process_tcp.is_alive() == True:
                process_tcp.terminate()
        if process_icmp != NONE:
            if process_icmp.is_alive() == True:
                process_icmp.terminate()
        if process_udp != NONE:
            if process_udp.is_alive() == True:
                process_udp.terminate()
        process_igmp = Process(target = igmp)
        process_igmp.start()
'''
def printkey(event):

    if event.char == "1":   #icmp
        global icmp_package
        if icmp_package == NONE:
            icmp_package = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", icmp_package))
    if event.char == "2":   #tcp
        global tcp_package
        if tcp_package == NONE:
            tcp_package = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", tcp_package))
    if event.char == "3":   #udp
        global udp_package
        if udp_package == NONE:
            udp_package = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", udp_package))
    if event.char == "4":   #igmp
        global igmp_package
        if igmp_package == NONE:
            igmp_package = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", igmp_package))
    if event.char == "5":   #arp
        global arp_package_content
        if arp_package_content == NONE:
            arp_package_content = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", arp_package_content))
    if event.char == "6":   #rarp
        global rarp_package_content
        if rarp_package_content == NONE:
            rarp_package_content = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", rarp_package_content))
    if event.char == "a":   #all package
        global all_package
        if all_package == NONE:
            all_package = ""
        text.delete(0.0, END)
        root.bind("<<COMRxRdy>>", lambda evt: text.insert("insert", all_package))

lock = Lock()

thread_icmp = Thread(target = icmp)
thread_tcp = Thread(target = tcp)
thread_udp = Thread(target = udp)
thread_igmp = Thread(target = igmp)
thread_arp = Thread(target = arp)
thread_rarp = Thread(target = rarp)
thread_icmp.start()
thread_tcp.start()
thread_udp.start()
thread_igmp.start()
thread_arp.start()
thread_rarp.start()
#root = Tk()
text = Text(root)
entry = Entry(root)
entry.bind('<Key>', printkey)
text.bind(root)
entry.pack()
text.pack()
root.mainloop()
