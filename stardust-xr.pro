QT += qml quick quick3d-private widgets openglextensions
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/core/configpathgetter.cpp \
    src/core/fileio.cpp \
    src/core/launcher.cpp \
    src/core/prefs.cpp \
    src/core/extensionloader.cpp \
    src/keyboard/recoverykeyboardadapter.cpp \
    src/openxr/stardustopengl.cpp \
    src/openxr/stardustopenxr.cpp \
    src/openxr/stardustopenxrframe.cpp \
    src/openxr/stardustopenxrgraphics.cpp \
#    src/pointer/inputpointer.cpp \
    src/keyboard/keyboard.cpp \
    src/keyboard/keyboardhandler.cpp \
    src/keyboard/passthroughkeyboardhandler.cpp \
    src/core/pluginloader.cpp \
    src/main.cpp

RESOURCES += src/qml.qrc \
    defaults/defaults.qrc \
    resources/resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=	-i/usr/local/include

LIBS += -lopenxr_loader \
        -lGLEW \
        -lglfw \
        -lX11 \
        -linput

HEADERS += \
    src/core/configpathgetter.h \
    src/core/fileio.h \
    src/core/launcher.h \
    src/core/plugininterfaces.h \
    src/core/prefs.h \
    src/core/extensionloader.h \
    src/keyboard/recoverykeyboardadapter.h \
    src/openxr/opengl_meta.h \
    src/openxr/openxr_meta.h \
    src/openxr/stardustopengl.h \
    src/openxr/stardustopenxr.h \
    src/openxr/stardustopenxrframe.h \
    src/openxr/stardustopenxrgraphics.h \
#    src/pointer/inputpointer.h \
    src/keyboard/keyboard.h \
    src/keyboard/keyboardhandler.h \
    src/keyboard/passthroughkeyboardhandler.h \
    src/core/pluginloader.h
