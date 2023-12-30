import socket

host = '0.0.0.0'
port = 8080

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))
server_socket.listen(5)


def authenticate_token(_token: str) -> bool:
    return False

def send_ok(client):
    client.send(b"HTTP/1.0 200 OK")

def send_unauthorized(client):
    client.send(b"HTTP/1.0 401 Unauthorized")


print(f"Server listening on {host}:{port}")

while True:
    client_socket, client_address = server_socket.accept()

    print(f"Connection from {client_address}")

    data = client_socket.recv(1024)
    if not data:
        break

    text = data.decode('utf-8')

    index = text.index("Bearer ")
    buitenste_index = text[index:].index("\r\n")

    start = index + 7
    einde = buitenste_index + index
    bearer_header = text[start:einde]

    print(bearer_header, len(bearer_header))

    if authenticate_token(bearer_header):
        send_ok(client_socket)
    else:
        send_unauthorized(client_socket)


    client_socket.close()

# Close the server socket
server_socket.close()

