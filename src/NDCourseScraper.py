
import requests
from bs4 import BeautifulSoup
import json
import argparse

def getSubjects():
    '''scrape a list of subject codes for ND courses'''

    url = "https://class-search.nd.edu/reg/srch/ClassSearchServlet"
    html = requests.post(url).text

    # get the ids from the subject select box
    subjects = {}
    subjNode = BeautifulSoup(html, 'lxml').find('select', id='SUBJ')
    subjOptions = subjNode.find_all('option')
    for option in subjOptions:
        subjects[option['value']] = option.text.strip()
    
    return subjects

def parseCourseSec(s):
    '''Break the "Course - Sec" string into its components (deparment, course #,
    and section #). Return a dictionary of these or None on a parsing error'''

    department = ""
    courseNum = ""
    sectionNum = ""

    # get deparment code
    i = 0
    while i < len(s):
        if s[i].isdigit():
            break

        department += s[i]
        i += 1
    
    # get course number
    while i < len(s):
        if not s[i].isdigit():
            break
        
        courseNum += s[i]
        i += 1
    
    # skip to start of section number
    while i < len(s):
        if s[i].isdigit():
            break
        i += 1
    
    # get section number
    while i < len(s):
        if not s[i].isdigit():
            break
        sectionNum += s[i]
        i += 1
    
    # check for valid results
    if len(department) == 0 or len(courseNum) == 0 or len(sectionNum) == 0:
        return None
    
    return {
        "department" : department,
        "course" : int(courseNum),
        "section" : int(sectionNum)
    }

def parseTimes(s):
    '''Parse the times column (labeled "When" on the website) into a list of
    days and the start and end times. Return a dictionary of the values or None
    if there's a parsing error'''

    days = []
    start = ""
    end = ""

    # skip anything before the list of days
    i = 0
    while i < len(s):
        if s[i].isalpha():
            break
        i += 1

    # parse the days, represent Monday through Friday as 0 - 4
    while i < len(s):
        if s[i].isdigit():
            break
        elif s[i].isalpha():
            try:
                days.append('MTWRF'.index(s[i]))
            except:
                return None

        i += 1
    
    # parse the start time
    while i < len(s):
        if s[i] == ':':
            pass
        elif s[i].isdigit():
            start += s[i]
        else:
            # perform conversion to military time
            start = int(start)
            if start >= 1200:
                start -= 1200

            if s[i] == 'P':
                start += 1200
            elif s[i] != 'A':
                return None
            
            break
        
        i += 1

    # skip to the end time
    while i < len(s):
        if s[i].isdigit():
            break
        i += 1
    
    # parse the end time
    while i < len(s):
        if s[i] == ':':
            pass
        elif s[i].isdigit():
            end += s[i]
        else:
            # perform conversion to military time
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
    '''Parse the location field ("Where" column) by removing extra whitespace'''

    loc = ", ".join(part.strip() for part in s.split('\n') if not part.isspace())
    return {"location" : loc}        

def getSubjectCatalogue(subjectCode):
    '''get a list of course section for a given subject code. Return a list of
    dictionaries'''

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

    # check if no results were returned
    if table is None:
        return courses

    # iterate through each row of the table body
    trs = table.find('tbody').find_all('tr')
    for courseRow in trs:

        # extract the row DOM nodes
        cols = courseRow.find_all('td')
        
        if len(cols) != 14:
            continue

        # get the column values we're interested in
        courseSec = cols[0].text.strip().split('\n')[0]
        title = cols[1].text.strip()
        crn = cols[7].text.strip()
        instructor = cols[9].text.strip()
        times = cols[10].text.strip().split('\n')[0]
        location = cols[13].text

        # peform extra parsing and combine all the dictionaries
        courseSec = parseCourseSec(courseSec)
        title = {"title" : title}
        crn = {"crn": int(crn)}
        instructor = {"instructor" : instructor}
        times = parseTimes(times)
        location = parseLocation(location)

        # discard the entry if a parsing error occurred
        if courseSec is None or times is None:
            continue

        # combine all the dictionaries into courseInfo
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
    '''get a course catalogue for all courses in all subjects on main campus;
    return a list of dictionaries'''

    # get the courses for each subject and combine them together
    subjects = getSubjects()
    courses = []
    for code, subj in subjects.items():
        subjCourses = getSubjectCatalogue(code)
        courses.extend(subjCourses)

        if progressMessages:
            print(f'Got {len(subjCourses)} courses for {subj} ({code})')
    
    return courses

def convertCourses(courses):
    '''Convert the courses into the format used by the rest of the program; 
    group together sections of the same course'''

    depCodes = list(getSubjects().keys())

    converted = {}
    for course in courses:

        # combine the department code and the course number to create a unique
        # course ID
        courseID = depCodes.index(course['department']) * 100000 + course['course']

        # use the dictionary so that all sections of the same course are grouped
        # together (separate sections have their own entry in the 'times' field)
        if courseID not in converted:
            converted[courseID] = {
                "id" : courseID,
                "name" : course['title'],
                "times" : []
            }
        
        # convert times into the format of minutes since midnight of Monday
        # morning
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

# take the first command line argument to determine the desired output file
parser = argparse.ArgumentParser()
parser.add_argument('file', type=str, help="name to write json contents to")
args = parser.parse_args()

# scrape the courses and write them to the specified output file
courses = getAllCourses(progressMessages=True)
converted = convertCourses(courses)
with open(args.file, 'w') as fp:
    json.dump(converted, fp, indent=4)
