#!/bin/sh
~/Dev/bash/status &
~/.fehbg &
nm-applet &
setxkbmap -layout de,gr
lxpolkit &
picom --experimental-backends -b &
xautolock -time 20 -locker i3lock-fancy &
dwm
