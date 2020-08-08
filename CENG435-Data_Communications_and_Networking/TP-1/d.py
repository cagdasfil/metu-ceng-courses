from socket import socket, AF_INET, SOCK_DGRAM
from sys import argv, stdout
#from common import ip_checksum

def ip_checksum(data):  # Form the standard IP-suite checksum
    pos = len(data)
    if (pos & 1):  # If odd...
        pos -= 1
        sum = ord(data[pos])  # Prime the sum with the odd end byte
    else:
        sum = 0

    #Main code: loop to calculate the checksum
    while pos > 0:
        pos -= 2
        sum += (ord(data[pos + 1]) << 8) + ord(data[pos])

    sum = (sum >> 16) + (sum & 0xffff)
    sum += (sum >> 16)

    result = (~ sum) & 0xffff  # Keep lower 16 bits
    result = result >> 8 | ((result & 0xff) << 8)  # Swap bytes
    return chr(result / 256) + chr(result % 256)


def send(content, to):
    checksum = ip_checksum(content)
    send_sock.sendto(checksum + content, to)

if __name__ == "__main__":
    dest_addr = argv[1]
    dest_port = int(argv[2])
    dest = (dest_addr, dest_port)
    listen_addr = argv[3]
    listen_port = int(argv[4])
    listen = (listen_addr, listen_port)

    send_sock = socket(AF_INET, SOCK_DGRAM)
    recv_sock = socket(AF_INET, SOCK_DGRAM)

    recv_sock.bind(listen)

    expecting_seq = 0

    f = open("output.txt","w")

    while True:
        message, address = recv_sock.recvfrom(4096)

        checksum = message[:2]
        seq = message[2]
        content = message[3:]

        if ip_checksum(content) == checksum:
            send("ACK" + seq, dest)
            if seq == str(expecting_seq):
                #stdout.write(content)
                f.write(content)
                expecting_seq = 1 - expecting_seq
        else:
            negative_seq = str(1 - expecting_seq)
            send("ACK" + negative_seq, dest)

    f.close()