## -*- coding: utf-8 -*-

from PyQt5 import uic

pyfile = open("pymainwindow.py", 'w')
uic.compileUi("mainwindow.ui", pyfile)
pyfile.close()
