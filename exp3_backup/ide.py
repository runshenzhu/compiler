import os
import wx
# -*- coding: utf-8 -*-  

def load(event):
    try:
            with open(Filename.GetValue()) as f:
                    print 'in load ok'
                    Contents.SetValue(f.read())
                    AST.SetValue('')
                    Result.SetValue('')
    except IOError:
                    print 'in null file'
                    Contents.SetValue('')
                    AST.SetValue('')
                    Result.SetValue('')                    
                    return


def save(event):
    with open(Filename.GetValue(), 'w+') as f:
            f.write(Contents.GetValue())

def run(event):
    cmd = "./compiler "+Filename.GetValue()+" >result.txt"
    os.system(cmd) 
    with open("./result.txt") as f:
        ASTBuf=''
        ResultBuf=''
        f.readline();
        for line in f:
            if line.find('*****')==0:
                break
            else:
                ASTBuf+=line

        for line in f:
            ResultBuf+=line
        
        AST.SetValue(ASTBuf)
        Result.SetValue(ResultBuf)

app = wx.App()
win = wx.Frame(None, title='C-- Simple IDE', size = (820, 670))
BKG = wx.Panel(win)
OpenBtn = wx.Button(BKG, label = 'Open')
OpenBtn.Bind(wx.EVT_BUTTON, load)

SaveBtn = wx.Button(BKG, label = 'Save')
SaveBtn.Bind(wx.EVT_BUTTON, save)

RunBtn = wx.Button(BKG, label = 'Run')
RunBtn.Bind(wx.EVT_BUTTON, run)

Filename = wx.TextCtrl(BKG)
Filename.SetValue('./example.txt')

Contents = wx.TextCtrl(BKG, style = wx.TE_MULTILINE | wx.HSCROLL)
AST =  wx.TextCtrl(BKG, style = wx.TE_MULTILINE | wx.HSCROLL)
Result =  wx.TextCtrl(BKG, style = wx.TE_MULTILINE | wx.HSCROLL)


hbox = wx.BoxSizer()
hbox.Add(Filename, proportion = 1, flag = wx.EXPAND)
hbox.Add(OpenBtn, proportion = 0, flag = wx.LEFT, border = 5)
hbox.Add(SaveBtn, proportion = 0, flag = wx.LEFT, border = 5)
hbox.Add(RunBtn, proportion = 0, flag = wx.LEFT, border = 5)


c1 = wx.BoxSizer()
c1.Add(Contents, proportion = 4, flag = wx.EXPAND)
c1.Add(AST, proportion = 1, flag = wx.EXPAND)

vbox = wx.BoxSizer(wx.VERTICAL)
vbox.Add(hbox, proportion = 0, flag = wx.EXPAND | wx.ALL, border = 5)
vbox.Add(c1, proportion = 3, flag = wx.EXPAND | wx.LEFT | wx.BOTTOM | wx.RIGHT, border = 5)
vbox.Add(Result, proportion = 1, flag = wx.EXPAND | wx.LEFT | wx.BOTTOM | wx.RIGHT, border = 5)


BKG.SetSizer(vbox)

win.Show()
app.MainLoop()
