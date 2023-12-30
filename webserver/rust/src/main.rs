use std::{
    io::{BufReader, Read, Write},
    net::{TcpListener, TcpStream},
};

fn main() {
    let tcp = TcpListener::bind("0.0.0.0:8081").unwrap();

    for client in tcp.incoming() {
        if let Ok(stream) = client {
            handle_stream(stream);
        }
    }
}

fn handle_stream(client: TcpStream) {
    let mut buff_reader = BufReader::new(client);
    let mut buffer = [0u8; 1024];

    let read = buff_reader.read(&mut buffer).unwrap();
    let request = std::str::from_utf8(&buffer[0..read]).unwrap();

    let Some(index) = request.find("Bearer ") else {
        return;
    };
    let Some(buitense_index) = &request[index..read].find("\r\n") else {
        return;
    };
    let bearer_token = &request[index + 7..index + buitense_index];

    println!("{}", bearer_token);

    let is_authenticated = validate_token(bearer_token);

    if is_authenticated {
        return_ok(buff_reader.into_inner());
    } else {
        return_unauthorized(buff_reader.into_inner());
    }
}

fn validate_token(_token: &str) -> bool {
    false
}

fn return_ok(mut stream: TcpStream) {
    stream.write_all(b"HTTP/1.1 200 OK\r\n").unwrap();
}
fn return_unauthorized(mut stream: TcpStream) {
    stream.write_all(b"HTTP/1.1 401 Unauthorized\r\n").unwrap();
}
