import requests as r
import sys
import random
import string

num = int(sys.argv[1])

letters = string.ascii_lowercase
result_str = ''.join(random.choice(letters) for i in range(num))

print("Payload:", result_str)

try:
    response = r.get("http://localhost:8081", headers={
        "Authorization" : f"Bearer {result_str}"
        })
except:
    print("Request failed")
    sys.exit(1)

# print(response.raw.read(10))
print(response.status_code)
print(response.content)
