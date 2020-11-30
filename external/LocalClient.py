import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 3579))
s.send('Hello windows!'.encode())
r = s.recv(1024)
print(r.decode())