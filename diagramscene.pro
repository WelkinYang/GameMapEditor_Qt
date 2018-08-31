QT += widgets
QT += xlsx
requires(qtConfig(fontcombobox))
requires(qtConfig(tableview))

HEADERS	    =   mainwindow.h \
		diagramitem.h \
		diagramscene.h \
		arrow.h \
		diagramtextitem.h \
    diagramellipseitem.h \
    itemsignalemit.h \
    diagramview.h \
    diagramdashitem.h
SOURCES	    =   mainwindow.cpp \
		diagramitem.cpp \
		main.cpp \
		arrow.cpp \
		diagramtextitem.cpp \
		diagramscene.cpp \
    digramellipseitem.cpp \
    itemsignalemit.cpp \
    diagramview.cpp \
    diagramdashitem.cpp
RESOURCES   =	diagramscene.qrc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target
