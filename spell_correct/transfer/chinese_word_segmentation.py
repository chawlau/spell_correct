import urllib2
import sys,time
import sys
sys.path.append("../")
import jieba
jieba.initialize()

url = sys.argv[1]
content = open(url,"rb").read()
t1 = time.time()
words = " ".join(jieba.cut(content))

t2 = time.time()
tm_cost = t2-t1
url1=sys.argv[2];
log_f = open(url1,"wb")
log_f.write(words.encode('utf-8'))
log_f.close()

print 'cost',tm_cost
print 'speed' , len(content)/tm_cost, " bytes/second"
