#!/bin/bash
~/.fehbg &
/usr/libexec/xfce4/notifyd/xfce4-notifyd &
picom -b &
~/Src/dwm/scripts/sched &
~/Dev/bash/weather &
~/Dev/bash/radio &
nm-applet &
udiskie -t &
blueman-applet &
setxkbmap -layout de,gr -option caps:escape
xrandr --auto --output DP-2 --mode 1920x1080 --right-of DP-0
