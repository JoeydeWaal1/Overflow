docker run --rm -p 8081:8081 -v "$PWD":/usr/src/myapp -w /usr/src/myapp gcc:4.9 make run
