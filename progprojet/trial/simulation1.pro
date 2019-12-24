TEMPLATE = subdirs

SUBDIRS += \
   general \
   Qt_GL

Qt_GL.depends = general
