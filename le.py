f = 1
f <<= 256
f -= 1

print(len(str(f)))
print(str(f)[::-1])

for s in reversed(str(f)):
    print(f"{s}", end=", ")
