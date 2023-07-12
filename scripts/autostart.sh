#!/bin/bash
~/.fehbg &
xrandr --auto --output DP-2 --mode 1920x1080 --right-of DP-0
/usr/libexec/xfce4/notifyd/xfce4-notifyd &
picom -b &
~/Src/dwm/scripts/sched &
~/Dev/bash/weather &
~/Dev/bash/radio &
systemctl --user import-environment; systemctl --user start xsession.target
nm-applet &
udiskie -t &
blueman-applet &
/usr/libexec/polkit-gnome-authentication-agent-1 &
setxkbmap -layout de,gr -option caps:escape
