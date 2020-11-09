
import subprocess

def parseLine(line):
    line = line.strip()

    data = {}
    colNames = ['percent', 'total', 'self', 'calls']

    i = 0
    for col in range(6):
        num = ""
        while i < len(line) and (line[i].isdigit() or line[i] == '.'):
            num += line[i]
            i += 1
        
        while i < len(line) and line[i].isspace():
            i += 1
        
        if len(num) == 0:
            return None
        
        if col < 4:
            data[colNames[col]] = float(num)
    
    data['name'] = line[i:]

    return data


gprofProcess = subprocess.Popen(['gprof', 'exe/EventSchedulerPerfTest', '--flat-profile'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
subStdout, subStderr = gprofProcess.communicate()

funcInfo = []
lines = subStdout.decode('utf-8').split('\n')
for line in lines:
    fInfo = parseLine(line)
    if fInfo is not None:
        funcInfo.append(fInfo)

funcInfo.sort(key = lambda x: x['self'], reverse=True)

for fi in funcInfo[:20]:
    print(fi)