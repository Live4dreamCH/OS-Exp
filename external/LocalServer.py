import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('127.0.0.1', 3579))
s.listen(5)
clientsocket,addr = s.accept()
clientsocket.send('Hello windows!'.encode())
r = clientsocket.recv(1024)
print(r.decode())