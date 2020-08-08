input_file = open("ptrace_input.txt",'r')
trace_file = open("ptrace.trc",'r')

line = trace_file.readline()

exp = ""
count = 0

while line:
    if float(line)>5:
        count = 0
        while float(line)>5:
            count+=1
            line = trace_file.readline()
        if count>100:
            exp+="1"
        else:
            exp+="0"
    line = trace_file.readline()

c = int(input_file.readline(), 16)
n = int(input_file.readline(), 16)

m = pow(c, int(exp, 2), n)

m_hex = hex(m)

plain_text = ""
for i in range(2,len(m_hex),2):
    plain_text += chr(int(m_hex[i:i+2],16))

print(plain_text)