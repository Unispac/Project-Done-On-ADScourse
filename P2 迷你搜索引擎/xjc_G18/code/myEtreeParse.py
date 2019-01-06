from lxml import etree
for i in range(45,992):#this is our document id number
    temp = i-39 #set it as the number I want
    myhtml = etree.parse('./html/'+str(i)+'.txt',etree.HTMLParser())
    title = myhtml.xpath('//title/text()')#parse the title
    print title
    newtitle = [i.replace('\n',' ')for i in title]
    print newtitle
    result = myhtml.xpath('//td/text()|//p/text()|//h1/text()|//h2/text()|//h3/text()|//blockquote/i/text()|//blockquote/a/text()|//a/b/text()|//p/i/text()|//address/text()|//td/a/text()|//body/blockquote/text()')#parse the content
    print result
    f = open('data//'+str(temp)+'.txt','w')# write the pure content in to data folder and save it
    for t in newtitle:
        f.write(t+'\n')
    for line in result:
        f.write(line)
    f.close()

