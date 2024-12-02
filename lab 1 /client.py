import socket
def send_file (filename, host='127.0.0.1', port=65432):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        with open(filename, "rb") as f:
            data = f.read(1024)
            while data:
                s.sendall(data)
                data = f.read(1024)
                print("send done")
                s.close()

if __name__ == "__main__":
    send_file("../1.txt")
