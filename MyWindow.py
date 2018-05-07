# -*- coding: utf-8 -*-

import sys
from PyQt5 import uic, QtWidgets


class MyWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MyWindow, self).__init__()
        uic.loadUi('mainwindow.ui', self)
        self.show()

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = MyWindow()
    sys.exit(app.exec_())
