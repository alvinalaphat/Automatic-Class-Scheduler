
import subprocess
import time

def parseLine(line):
    '''parse a line of gprof output (from the flat profiler), return a
    dictionary with the function's name, the percent of time spent in it, total
    time spent there, total time excluding subroutines, and number of calls'''

    line = line.strip()

    data = {}
    colNames = ['percent', 'total', 'self', 'calls']

    # parse each column
    i = 0
    for col in range(6):

        # parse a whitespace separated entry
        num = ""
        while i < len(line) and (line[i].isdigit() or line[i] == '.'):
            num += line[i]
            i += 1
        
        while i < len(line) and line[i].isspace():
            i += 1
        
        # if the format was incorrect, return None
        if len(num) == 0:
            return None
        
        # cast numeric columns to 
        if col < 4:
            data[colNames[col]] = float(num)
    
    # the remainder of the line is the name
    data['name'] = line[i:]

    return data

def getFunction(funcInfo, name):
    '''get the information for a specific function; a subsection of the name
    sufficient to uniquely identify the function must be provided'''

    for fi in funcInfo:
        try:
            fi['name'].index(name)
            return fi
        except ValueError:
            pass

def getTopFunctions(fi, n = 5):
    '''Get the n functions that took the greatest portion of processing time'''

    fi.sort(key = lambda s: s['self'], reverse=True)
    if len(fi) < n:
        return fi
    
    return fi[:n]

def perfTest(E, s, M):
    '''perform a single performance test with E events, s sections per event,
    and M schedules considered; return the parsed gprof output'''

    # runt the program
    testProcess = subprocess.Popen(['./exe/EventSchedulerPerfTest', str(E), str(s), str(M)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    testProcess.wait()

    # run gprof
    gprofProcess = subprocess.Popen(['gprof', 'exe/EventSchedulerPerfTest', '--flat-profile'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    subStdout, subStderr = gprofProcess.communicate()

    # parse gprof output
    funcInfo = []
    lines = subStdout.decode('utf-8').split('\n')
    for line in lines:
        fInfo = parseLine(line)
        if fInfo is not None:
            funcInfo.append(fInfo)

    return funcInfo

def timeTest(E, s, M):
    '''Test the total (real) time taken to execute the scheduling program'''

    t0 = time.time() # start timer

    # run te scheduilng program
    testProcess = subprocess.Popen(['./exe/EventSchedulerPerfTest', str(E), str(s), str(M)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    testProcess.wait()

    totalTime = time.time() - t0 #e nd timer

    return totalTime

def runTimingTests(tests):
    '''run multiple timing tests and print the results in a table'''

    print("E\ts\tM\ttime (s)")

    for test in tests:
        t = timeTest(test[0], test[1], test[2])
        print(test[0], test[1], test[2], t, sep="\t")

def runTopFunctions(tests):
    '''run multiple timing tests and print the top 5 costliest functions for
    each test'''

    for test in tests:
        print(f"E = {test[0]}, s = {test[1]}, M = {test[2]}")
        results = perfTest(test[0], test[1], test[2])
        results = getTopFunctions(results)
        for result in results:
            print('\t', result)

def runFunctionAnalysis(tests, funcName):
    '''run multiple tests and extract the time spent in a particular function
    for each test; print the results in a table'''

    print("E\ts\tM\ttotal\t%\tcalls")
    for test in tests:
        results = perfTest(test[0], test[1], test[2])
        results = getFunction(results, funcName)
        print(test[0], test[1], test[2], results['self'], results['percent'], results['calls'], sep='\t')


# default set of tests that vary E, s, and M
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

runTimingTests(tests)
#runTopFunctions(tests)
#runFunctionAnalysis(tests, 'sectionConflictsWithSchedule')