@echo off
SetLocal EnableDelayedExpansion
(set PATH=D:\software\anaconda\Library\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=D:\software\anaconda\Library\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=D:\software\anaconda\Library\plugins
)
%*
EndLocal
