export MakeInc_cmd=${SRCROOT}/makedefs/MakeInc.cmd
export MakeInc_def=${SRCROOT}/makedefs/MakeInc.def
export MakeInc_rule=${SRCROOT}/makedefs/MakeInc.rule
export MakeInc_dir=${SRCROOT}/makedefs/MakeInc.dir

include $(MakeInc_cmd)
include $(MakeInc_def)

INSTINC_SUBDIRS = IOKit DriverKit System IOKitUser Exclaves

EXPINC_SUBDIRS = IOKit DriverKit Exclaves

COMP_SUBDIRS = conf

include $(MakeInc_rule)
include $(MakeInc_dir)
