TEMPLATE = subdirs

SUBDIRS = yabr yabr-config yabr-lib

yabr.depends = yabr-lib
yabr-config.depends = yabr-lib
