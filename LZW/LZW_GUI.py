# -*- coding: UTF-8 -*-

__author__ = 'FinalTheory'

import Tkinter as tk
import tkMessageBox
from subprocess import call
from tkFileDialog import askopenfilename, asksaveasfilename


#界面逻辑部分
class GUI(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self.SrcName = tk.StringVar()
        self.DstName = tk.StringVar()

        self.title('LZW')
        self.protocol("WM_DELETE_WINDOW", lambda: (self.quit(), self.destroy()))
        self.frame = tk.Frame(self)
        tk.Label(self.frame, text=u'源文件名称').grid(row=0, column=0, sticky=tk.W)
        tk.Entry(self.frame, width=9, textvariable=self.SrcName).grid(row=0, column=1)
        tk.Button(self.frame, text='...', command=self.getSrcName).grid(row=0, column=2)
        tk.Label(self.frame, text=u'目标文件名称').grid(row=1, column=0, sticky=tk.W)
        tk.Entry(self.frame, width=9, textvariable=self.DstName).grid(row=1, column=1)
        tk.Button(self.frame, text='...', command=self.getDstName).grid(row=1, column=2)
        tk.Button(self.frame, text=u'压缩', width=6, command=self.Compress, font=('YaHei', 12)).grid(row=2, column=0)
        tk.Button(self.frame, text=u'解压', width=6, command=self.Decompress, font=('YaHei', 12)).grid(row=2, column=1)
        self.frame.grid()

    def getSrcName(self):
        filename = askopenfilename(title=u'选择源文件')
        self.SrcName.set(filename)

    def getDstName(self):
        filename = asksaveasfilename(title=u'选择目标文件名')
        self.DstName.set(filename)

    def Compress(self):
        if self.DstName.get() == "":
            self.DstName.set(self.SrcName.get() + ".lzw")
        command = 'LZW.exe -compress "' + self.SrcName.get() + '" "' + self.DstName.get() + '"'
        returnValue = call(command)
        if returnValue == 0:
            tkMessageBox.showinfo(u"恭喜你", u"文件压缩完成")
        else:
            tkMessageBox.showwarning(u"错误", u"压缩失败！")

    def Decompress(self):
        if self.DstName.get() == "":
            self.DstName.set('.'.join(self.SrcName.get().split('.')[:-1]))
        command = 'LZW.exe -decompress "' + self.SrcName.get() + '" "' + self.DstName.get() + '"'
        returnValue = call(command)
        if returnValue == 0:
            tkMessageBox.showinfo(u"恭喜你", u"文件解压完成")
        else:
            tkMessageBox.showwarning(u"错误", u"解压失败！")


if __name__ == "__main__":
    GUI().mainloop()
