import lvgl as lv
import json
import network

f=open('/data/graf.txt','r')
data = json.load(f)
f.close()

#############################
# funciones de los botones
#############################
def cls(e,ta,label):
    ta.set_text("")
    if(label!=None):
        label.set_text("")


