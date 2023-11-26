#!/bin/sh
xset s off && xset -dpms && xset s noblank
~/Dev/bash/status &
~/.fehbg &
~/Dev/bash/monitors &
blueman-applet &
nm-applet &
~/.local/share/gnome-shell/extensions/gsconnect@andyholmes.github.io/service/daemon.js &
setxkbmap -layout de,gr
lxpolkit &
picom --experimental-backends -b &
xautolock -time 20 -locker i3lock-fancy &
dwm
