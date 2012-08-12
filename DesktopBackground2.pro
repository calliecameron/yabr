TEMPLATE = subdirs

SUBDIRS = RandomBackground RandomBackgroundConfig RandomBackgroundLib

RandomBackground.depends = RandomBackgroundLib
RandomBackgroundConfig.depends = RandomBackgroundLib
