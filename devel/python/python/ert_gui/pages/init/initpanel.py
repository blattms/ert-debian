#  Copyright (C) 2011  Statoil ASA, Norway. 
#   
#  The file 'initpanel.py' is part of ERT - Ensemble based Reservoir Tool. 
#   
#  ERT is free software: you can redistribute it and/or modify 
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or 
#  (at your option) any later version. 
#   
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#  FITNESS FOR A PARTICULAR PURPOSE.   
#   
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
#  for more details. 


from PyQt4 import QtGui, QtCore
from ert_gui.widgets.tablewidgets import KeywordList
from ert_gui.widgets.validateddialog import ValidatedDialog
import ert.ert.ertwrapper as ertwrapper
from ert_gui.widgets.combochoice import ComboChoice


from ert_gui.widgets.helpedwidget import HelpedWidget
from ert_gui.widgets.util import resourceIcon, createSeparator, may_take_a_long_time

from initandcopy import *


class InitPanel(QtGui.QFrame):
    
    def __init__(self, parent):
        QtGui.QFrame.__init__(self, parent)
        self.setFrameShape(QtGui.QFrame.Panel)
        self.setFrameShadow(QtGui.QFrame.Raised)

        initPanelLayout = QtGui.QVBoxLayout()
        self.setLayout(initPanelLayout)

        casePanel = QtGui.QFormLayout()


        def get_case_list(ert):
            fs = ert.main.get_fs
            list = ert.main.alloc_caselist
            return list

        self.get_case_list = get_case_list # convenience: used by several functions


        casePanel.addRow("Current case:", self.createCurrentCaseCombo())
        casePanel.addRow("Cases:", self.createCaseList())


        parametersPanelLayout = QtGui.QHBoxLayout()
        parametersPanelLayout.addWidget(ParametersAndMembers(self))

        initPanelLayout.addLayout(casePanel)
        initPanelLayout.addWidget(createSeparator())
        initPanelLayout.addLayout(parametersPanelLayout)

    def casesUpdated(self):
        """Emit to all listeners that the a new case has been added or the current case has changed"""
        self.currentCase.modelEmit("casesUpdated()")

    def createCaseList(self):
        """Creates a list that enables the creation of new cases. Removal has been disabled."""
        cases = KeywordList(self, "", "init/case_list")

        cases.newKeywordPopup = lambda list : ValidatedDialog(cases, "New case", "Enter name of new case:", list).showAndTell()
        cases.addRemoveWidget.enableRemoveButton(False)  #todo: add support for removal
        cases.list.setMaximumHeight(150)


        def create_case(ert, cases):
            fs = ert.main.get_fs

            for case in cases:
                if not fs.has_dir(case):
                    fs.select_write_dir(case, True)
                    break

            self.currentCase.updateList(self.get_case_list(ert))
            self.currentCase.fetchContent()
            self.casesUpdated()

        cases.getter = self.get_case_list
        cases.initialize = self.get_case_list
        cases.setter = create_case

        return cases


    def createCurrentCaseCombo(self):
        """Creates the combo that enables selection of the current case"""
        self.currentCase = ComboChoice(self, ["none"], help="init/current_case_selection")
        self.currentCase.combo.setMinimumWidth(150)

        def initialize_cases(ert):
            self.currentCase.updateList(self.get_case_list(ert))

        self.currentCase.initialize = initialize_cases

        def get_current_case(ert):
            fs = ert.main.get_fs
            tmp = self.get_case_list(ert)
            currentCase = ert.main.get_current_fs
            print "The selected case is: " + currentCase
            return currentCase

        self.currentCase.getter = get_current_case

        @may_take_a_long_time
        def select_case(ert, case):
            case = str(case)
            if not case == "":
                ert.main.user_select_fs( case )
                self.casesUpdated()

        self.currentCase.setter = select_case

        return self.currentCase

    