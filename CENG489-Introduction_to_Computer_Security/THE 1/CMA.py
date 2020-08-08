import csv
import egcd

hexValues = []

with open('crackme.csv','rt')as file:
  data = csv.reader(file)
  for var, value in data:
      hexValues.append(value)

c1 = int(hexValues[0], 16)
c2 = int(hexValues[1], 16)
e1 = int(hexValues[2], 16)
e2 = int(hexValues[3], 16)
n  = int(hexValues[4], 16)

gcd, x, y = egcd.egcd(e1, e2)

c1_x = pow(c1, x, n)
c2_y = pow(c2, y, n)

m = (c1_x*c2_y)%n

m_hex = hex(m)

plain_text = ""
for i in range(2,len(m_hex),2):
    plain_text += chr(int(m_hex[i:i+2],16))

print(plain_text)
