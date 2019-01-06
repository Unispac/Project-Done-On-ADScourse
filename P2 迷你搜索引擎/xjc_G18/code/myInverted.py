#-*- coding:utf-8 -*-
import nltk
nltk.download('punkt')
nltk.download('stopwords')
from nltk.stem.porter import PorterStemmer
from nltk.corpus import stopwords
import os
import json
#input
#process stop words
def ridStopWords():
    for filename in os.listdir('data'):
        f = open('data//'+filename,'r')
        o = open('fdata//'+filename,'w')
        #去除stop words
        for line in f:
            text = nltk.word_tokenize(line)
            filterted_words = [word for word in text if word not in stopwords.words('English')]
            o.write(' '.join(filterted_words))
            o.write('\n')
        f.close()
        o.close()
        print filename
#stemming
def stemming():
    for fname in os.listdir('fdata'):
        f = open('fdata//'+fname,'r')
        o = open('pdata//'+fname,'w')
        each_list = []
        for line in f:
            p = line.split()
            for each in p:
                porter_stem = PorterStemmer()
                o.write(porter_stem.stem(each))
                o.write(" ")
            o.write("\n")
        f.close()
        o.close()
        print fname
#建立我自己的倒排索引
def createInvertedIndex():
    offset = 0#count the offset of that line
    for pname in os.listdir('pdata'):
        f = open('pdata//'+pname,'r')
        fileindex = pname[:-4]
        lineNum = 0#count the line number
        for line in f:
            lineNum = lineNum + 1
            offset = 0
            t = line.split()
            for word in t:
                offset = offset + 1
                if word not in dict:
                    item = [fileindex,lineNum,offset]
                    index = []
                    index.append(item)
                    dict[word] = index
                else:
                    item = [fileindex,lineNum,offset]
                    dict[word].append(item)
        f.close()
        print pname
    print "finish parse"
#将内容放入json文件中 以方便跟C++写好的数据结构进行对接
def index2Json():
    with open("index.json",'w') as f:
        json.dump(dict,f,indent=4)

    print "finish json"
#word_list = dict.items()
#word_list.sort(key = lambda items: items[0])

#简单搜索 可以之后增加去除stop words 还有 stemming
def simpleQuery():
    print "Please input the word (e.g. \"and\" ):\n"
    query = input()
    stemmer = PorterStemmer()
    query = stemmer.stem(query)
    import time
    s = time.time()
    if query not in dict:
        print "No solution"
    else:
        print dict[query]
    e = time.time()
    print e-s
#阙值搜索
def thresholdQuery():
    print "start"
    print "Please input the word (e.g. \"and\" ):\n"
    queryWord = input()#get the input
    stemmer1 = PorterStemmer()
    queryWord = stemmer1.stem(queryWord)#stemming
    if queryWord not in dict:#judge if in the queryWord
        print "No solution"
    else:
        querylist = dict[queryWord]
        #print querylist
        indexcount = 0
        countdict = {}
        #Find out the word appear number in file
        for fileindex in range(0,len(querylist)):
           # print fileindex
           # print "qf:"+querylist[fileindex][0]
            temp = querylist[fileindex][0]
            if temp not in countdict:#count the filename appear numbers
                indexcount = indexcount + 1
                countdict[temp] = int(indexcount)
            else:
                countdict[temp] = countdict[temp]+1
    #get item in dict
    myitem = countdict.items()
    #sort it
    mycountdict = sorted(myitem,key=lambda x:x[1])
    rangecount = 0 #control the range to print file
    flag = 0 #to choose the print out message
    while(1):
        if flag == 0:
            for item in range(rangecount*5,rangecount*5+5):
                for num in range(0,len(querylist)):
                    if querylist[num][0] == mycountdict[item][0]:
                        print querylist[num];
        flag = 0
        print "please input 'b'(back),'f'(forward),'q'(quit) or input \"filename.txt\" to open the file\n"
        q = input()
        directoryList = os.listdir('pdata')
        if q == 'b':
            if rangecount*5 >len(mycountdict):
                print "It's the bottom of list"
                continue
            else:
                rangecount = rangecount + 1
                continue
        elif q == 'f':
            if rangecount == 0:
                print "It's already the top"
                continue
            else:
                rangecount = rangecount - 1
        elif q =='q':
               break
        elif q in directoryList:
            import linecache
            print "please input line number:\n"
            g = input()
            mywork = linecache.getline("pdata//"+q,g)
            print mywork
            a = q[:-4]
            countdict[a] = countdict[a] - 1
            myitem = countdict.items()
            mycountdict = sorted(myitem, key=lambda x: x[1])
            flag = 1
        else:
            print "please input 'b'(back),'f'(forward),'q'(quit) or input \"filename.txt\" to open the file\n"
if __name__ == "__main__":
    dict = {}#save all the index in here
    countlist =[]
   # ridStopWords()
   # stemming()
    createInvertedIndex()
   # index2Json()
   # simpleQuery()
    thresholdQuery()

