
import requests
from bs4 import BeautifulSoup
import json
import argparse

def getSubjects():
    url = "https://class-search.nd.edu/reg/srch/ClassSearchServlet"
    html = requests.post(url).text

    subjects = {}
    subjNode = BeautifulSoup(html, 'lxml').find('select', id='SUBJ')
    subjOptions = subjNode.find_all('option')
    for option in subjOptions:
        subjects[option['value']] = option.text.strip()
    
    return subjects

def parseCourseSec(s):
    department = ""
    courseNum = ""
    sectionNum = ""

    i = 0
    while i < len(s):
        if s[i].isdigit():
            break

        department += s[i]
        i += 1
    
    while i < len(s):
        if not s[i].isdigit():
            break
        
        courseNum += s[i]
        i += 1
    
    while i < len(s):
        if s[i].isdigit():
            break
        i += 1
    
    while i < len(s):
        if not s[i].isdigit():
            break
        sectionNum += s[i]
        i += 1
    
    if len(department) == 0 or len(courseNum) == 0 or len(sectionNum) == 0:
        return None
    
    return {
        "department" : department,
        "course" : int(courseNum),
        "section" : int(sectionNum)
    }

def parseTimes(s):
    days = []
    start = ""
    end = ""

    i = 0
    while i < len(s):
        if s[i].isalpha():
            break
        i += 1

    while i < len(s):
        if s[i].isdigit():
            break
        elif s[i].isalpha():
            try:
                days.append('MTWRF'.index(s[i]))
            except:
                return None

        i += 1
    
    while i < len(s):
        if s[i] == ':':
            pass
        elif s[i].isdigit():
            start += s[i]
        else:
            start = int(start)
            if start >= 1200:
                start -= 1200

            if s[i] == 'P':
                start += 1200
            elif s[i] != 'A':
                return None
            
            break
        
        i += 1

    while i < len(s):
        if s[i].isdigit():
            break
        i += 1
    
    while i < len(s):
        if s[i] == ':':
            pass
        elif s[i].isdigit():
            end += s[i]
        else:
            end = int(end)
            if end >= 1200:
                end -= 1200

            if s[i] == 'P':
                end += 1200
            elif s[i] != 'A':
                return None
            
            break
        
        i += 1
    
    return {
        "days" : days,
        "start" : start,
        "end" : end
    }

def parseLocation(s):
    loc = ", ".join([part.strip() for part in s.split('\n') if not part.isspace()])
    return {"location" : loc}        

def getSubjectCatalogue(subjectCode):
    courses = []

    url = "https://class-search.nd.edu/reg/srch/ClassSearchServlet"
    courseReq = {
        "TERM":     "202010",
        "DIVS": 	"A",
        "CAMPUS":	"M",
        "SUBJ":      subjectCode,
        "ATTR": 	"0ANY",
        "CREDIT":  	"A"
    }

    html = requests.post(url, data=courseReq).text
    soup = BeautifulSoup(html, 'lxml')
    table = soup.find('table', id='resulttable')
    if table is None:
        return courses

    trs = table.find('tbody').find_all('tr')
    for courseRow in trs:
        cols = courseRow.find_all('td')
        
        if len(cols) != 14:
            continue

        courseSec = cols[0].text.strip().split('\n')[0]
        title = cols[1].text.strip()
        crn = cols[7].text.strip()
        instructor = cols[9].text.strip()
        times = cols[10].text.strip().split('\n')[0]
        location = cols[13].text

        courseSec = parseCourseSec(courseSec)
        title = {"title" : title}
        crn = {"crn": int(crn)}
        instructor = {"instructor" : instructor}
        times = parseTimes(times)
        location = parseLocation(location)

        if courseSec is None or times is None:
            continue

        courseInfo = {}
        courseInfo.update(courseSec)
        courseInfo.update(title)
        courseInfo.update(crn)
        courseInfo.update(instructor)
        courseInfo.update(times)
        courseInfo.update(location)

        courses.append(courseInfo)
    
    return courses

def getAllCourses(progressMessages=False):
    subjects = getSubjects()
    courses = []
    for code, subj in subjects.items():
        subjCourses = getSubjectCatalogue(code)
        courses.extend(subjCourses)

        if progressMessages:
            print(f'Got {len(subjCourses)} courses for {subj} ({code})')
    
    return courses

def convertCourses(courses):
    depCodes = list(getSubjects().keys())

    converted = {}
    for course in courses:
        courseID = depCodes.index(course['department']) * 100000 + course['course']

        if courseID not in converted:
            converted[courseID] = {
                "id" : courseID,
                "name" : course['title'],
                "times" : []
            }
        
        convertedTimes = []
        start = course['start']
        end = course['end']
        for day in course['days']:
            convertedTimes.append([
                day * 1440 + start // 100 * 60 + start % 100,
                day * 1440 + end // 100 * 60 + end % 100
            ])

        converted[courseID]['times'].append(convertedTimes)
    
    converted = list(converted.values())
    return converted

parser = argparse.ArgumentParser()
parser.add_argument('file', type=str, help="name to right json contents to")
args = parser.parse_args()

courses = getAllCourses(progressMessages=True)
converted = convertCourses(courses)
with open(args.file, 'w') as fp:
    json.dump(converted, fp, indent=4)
