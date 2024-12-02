import socket


def start_server(host='127.0.0.1', port=65432):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print("Listen =))))")
        conn, add  = s.accept()
        with conn:
            print(f"Connect by {add}")
            with open("../1.txt", "wb") as f:
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    f.write(data)
            print("flie send")
            conn.close()
if __name__ == "__main__":
    start_server()