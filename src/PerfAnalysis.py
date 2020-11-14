
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

def getFunction(funcInfo, name):
    for fi in funcInfo:
        try:
            fi['name'].index(name)
            return fi
        except ValueError:
            pass

def perfTest(E, s, M):
    testProcess = subprocess.Popen(['./exe/EventSchedulerPerfTest', str(E), str(s), str(M)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    testProcess.wait()
    gprofProcess = subprocess.Popen(['gprof', 'exe/EventSchedulerPerfTest', '--flat-profile'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    subStdout, subStderr = gprofProcess.communicate()

    funcInfo = []
    lines = subStdout.decode('utf-8').split('\n')
    for line in lines:
        fInfo = parseLine(line)
        if fInfo is not None:
            funcInfo.append(fInfo)

    return getFunction(funcInfo, 'sectionConflictsWithSchedule')

tests = [
    [100, 5, 1000],
    [200, 5, 1000],
    [400, 5, 1000],
    [800, 5, 1000],
    [100, 10, 1000],
    [100, 20, 1000],
    [100, 40, 1000],
    [100, 80, 1000],
    [100, 5, 2000],
    [100, 5, 4000],
    [100, 5, 8000],
    [100, 5, 16000],
]

print("E\ts\tM\ttotal\t%\tcalls")
for test in tests:
    #print(f"E = {test[0]}, s = {test[1]}, M = {test[2]}")

    results = perfTest(test[0], test[1], test[2])
    
    print(test[0], test[1], test[2], results['self'], results['percent'], results['calls'], sep='\t')