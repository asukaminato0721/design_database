import json

# 词法分析 读取sql.txt文件中的程序进行分析
# 1-50 表示保留字种别码，在这里添加系统的关键字
# 部分还是C++程序的关键字保留字，可以删除
#完善SQL关键字
reserveWord = [
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while", "bool", "return", "false", "else", "char",
    "select", "from", "where","and","or","not",
    "as","between"]
# 51-90表示操作符种别码 这里一共36种
operatorOrDelimiter = [
    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
    "!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
    "&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
    "}", "\\", ".", "\\?", ":", "!"
]
# 99 表示常数种别码
# 100 表示标识符种别码


# 查找保留字
def searchReserve(s):
    for i in range(len(reserveWord)):
        if getStr(s) == reserveWord[i]:
            return i + 1
    return -1


# 查找操作符
def searchOperator(s):
    for i in range(len(operatorOrDelimiter)):
        if getStr(s) == operatorOrDelimiter[i]:
            return 50+i
    return -1


# 判断是否为字母
def IsLetter(letter:str)->bool:
    # if ('a' <= letter <= 'z') or ('A' <= letter <= 'Z'):
    #     return True
    # else:
    #     return False
    return letter.isalpha()


# 判断是否为数字
def IsDigit(digit:str)->bool:
    # if '0' <= digit <= '9':
    #     return True
    # else:
    #     return False
    return digit.isdigit()


# 编译预处理，取出无用的字符和注释
def filterResource(r, pProject):
    tempString = []
    count = 0
    i = 0
    while i < pProject:
        if r[i] == '/' and r[i + 1] == '/':
            while r[i] != '\n':
                # print("注释", i)
                i += 1
        elif r[i] == '/' and r[i + 1] == '*':
            i += 2
            while r[i] != '*' or r[i + 1] != '/':
                # print("注释", i)
                i = i + 1
                if r[i] == '$':
                    print("注释出错，没有找到*/，程序结束!!!")
                    exit(0)
            i += 2
        elif r[i] != '\n' and r[i] != '\t' and r[i] != '\v' and r[i] != '\r':
            # print("录入", i)
            tempString.append(r[i])
            i += 1
        else:
            i += 1
    tempString.append('\0')
    r = tempString
    return r


def checkCalc(ch:str)->bool:
    # if (ch == '+' or ch == '-' or ch == '*' or ch == '/' or ch == ';' or ch == '(' or ch == ')' or ch == '^'
    #         or ch == ',' or ch == '\"' or ch == '\'' or ch == '~' or ch == '#' or ch == '%' or ch == '['
    #         or ch == ']' or ch == '{' or ch == '}' or ch == '\\' or ch == '.' or ch == '\?' or ch == ':'):
    #     return True
    # else:
    #     return False
    return ch in { '+' , '-' , '*' , '/' , ';' , '(' , ')' , '^'
            , ',' , '\"' , '\'' , '~' , '#' , '%' , '['
            , ']' , '{' , '}' , '\\' , '.' , '\?' , ':'}


# 分析子程序，算法核心
# 根据DFA的状态转换图设计
# syn为种别码
# resourseProject 是资源文件
# token是每次扫描识别的单词
# pProject当前程序识别到的位置，下一次取单词从上一次的结束处开始，避免每次都从头开始扫描
def Scanner(syn, resourseproject, pProject):
    token = []
    count = 0  # count用来做token[]的指示器， 收集有用字符
    ch = resourseproject[pProject]  # ch用来作为判断使用
    # 过滤掉空格，防止程序因识别不了空格而结束
    while ch == ' ':
        pProject += 1
        ch = resourseproject[pProject]
    # 每次收集前先清0，每次识别下一个单词，在开始之前，吧之前识别的清空
    # for i in range(20):
    #     token.append('\0')
    # 开头为字母，则进行标识符或者关键字识别
    if IsLetter(ch):
        token.append(resourseproject[pProject])  # 收集一个字符
        pProject += 1  # 向后移动一位
        count += 1  # 单词字符数+1
        # 继续向后识别字符或数字，因为标识符可以由字符或数字组成
        while IsLetter(resourseproject[pProject]) or IsDigit(resourseproject[pProject]):
            # 收集字符
            token.append(resourseproject[pProject])  # 收集一个字符
            count += 1
            pProject += 1
        token.append('\0')
        # 查表找到种别码
        syn = searchReserve(token)
        # 若不是保留字则是标识符
        if syn == -1:
            syn = 100  # 标识符种别码
        return syn, token, pProject
    # 首字符为数字， 则可以肯定接下来的单词是一个数字
    elif IsDigit(ch):
        # 后面跟着也是字符， 此处开始读取数字
        while IsDigit(resourseproject[pProject]):
            token.append(resourseproject[pProject])
            count += 1
            pProject += 1
        token.append('\0')
        syn = 99
        return syn, token, pProject
    # 若为运算符或者界符，查表得到结果
    elif checkCalc(ch):
        token.append(resourseproject[pProject])
        token.append('\0')
        for i in range(len(operatorOrDelimiter)):
            if token == operatorOrDelimiter[i]:
                syn = 50 + i
                break
        pProject += 1  # 指针下移，为下一扫描做准备
        return syn, token, pProject
    # 接下来判断是否为> < >= <=
    elif resourseproject[pProject] == '<':
        token.append('<')
        pProject += 1
        if resourseproject[pProject] == '=':
            token.append('=')
            token.append('\0')
            syn = searchOperator(token)
        elif resourseproject[pProject] == '<':
            token.append('<')
            token.append('\0')
            pProject -= 1
            syn = searchOperator(token)
        else:
            token.append('\0')
            pProject -= 1
            syn = searchOperator(token)
        pProject += 1
        return syn, token, pProject
    elif resourseproject[pProject] == '>':
        token.append('>')
        pProject += 1
        if resourseproject[pProject] == '=':
            token.append('=')
            token.append('\0')
            syn = searchOperator(token)
        elif resourseproject[pProject] == '>':
            token.append('>')
            token.append('\0')
            syn = searchOperator(token)
            pProject -= 1
        else:
            pProject -= 1
            token.append('\0')
            syn = searchOperator(token)
        pProject += 1
        return syn, token, pProject
    elif resourseproject[pProject] == '=':
        token.append('=')
        pProject += 1
        if resourseproject[pProject] == '=':
            token.append('=')
            token.append('\0')
            syn = searchOperator(token)
        else:
            pProject -= 1
            token.append('\0')
            syn = searchOperator(token)
        pProject += 1
        return syn, token, pProject
    elif resourseproject[pProject] == '!':
        token.append('!')
        pProject += 1
        if resourseproject[pProject] == '=':
            token.append('=')
            token.append('\0')
            syn = searchOperator(token)
        else:
            pProject -= 1
            token.append('\0')
            syn = searchOperator(token)
        pProject += 1
        return syn, token, pProject
    elif resourseproject[pProject] == '&':
        token.append('&')
        pProject += 1
        if resourseproject[pProject] == '&':
            token.append('&')
            token.append('\0')
            syn = searchOperator(token)
        else:
            pProject -= 1
            token.append('\0')
            syn = searchOperator(token)
        pProject += 1
        return syn, token, pProject
    elif resourseproject[pProject] == '|':
        token.append('|')
        pProject += 1
        if resourseproject[pProject] == '|':
            token.append('|')
            token.append('\0')
            syn = searchOperator(token)
        else:
            pProject -= 1
            token.append('\0')
            syn = searchOperator(token)
        pProject += 1
        return syn, token, pProject
    elif resourseproject[pProject] == '$':  # 结束符
        syn = 0
    else:
        print("error：there is no exist")
        exit(0)
    return syn, token, pProject


def writeStr(str):
    with open("write.txt", 'a', encoding='utf-8') as f:
        f.write(str + '\n')


def getStr(token):
    str = ""
    for i in range(len(token)-1):
        str += token[i]
    return str


token = []
syn = -1
pProject = 0
with open("sql.txt", 'r', encoding='utf-8') as f:
    resourceProject = f.read()
resourceProject.__add__('\0')
print("过滤前的源程序：")
print(resourceProject)
resourceProject = filterResource(resourceProject, len(resourceProject) - 1)
print("过滤后的程序为：")
print(resourceProject)
IdentifierTbl = []
for i in range(1000):
    IdentifierTbl.append("")
while syn != 0:
    syn, token, pProject = Scanner(syn, resourceProject, pProject)
    if syn == 100:
        for i in range(1000):
            if IdentifierTbl[i] == token:
                break
            if IdentifierTbl[i] == "":
                IdentifierTbl[i] = token
                break
        print("标识符：", getStr(token))
        writeStr("(标识符 ," + getStr(token) + " )")
    elif 1 <= syn <= len(reserveWord):
        print("关键字:(%s , --)" % reserveWord[syn - 1])
        writeStr("(" + reserveWord[syn - 1] + " , --)")
    elif syn == 99:
        print("常数：(%s , --)" % getStr(token))
        writeStr("(常数 , " + getStr(token) + ")")
    elif 50 <= syn <= 98:
        print("(%s , --)" % operatorOrDelimiter[syn - 50])
        writeStr("(" + operatorOrDelimiter[syn - 50] + " , --)")
for i in range(100):
    print("第%d个标识符:%s" % (i + 1, IdentifierTbl[i]))
    writeStr("第" + i + "个标识符: " + IdentifierTbl[i])
