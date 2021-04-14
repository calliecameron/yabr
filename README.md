yabr: Yet Another (Desktop) Background Randomiser
=================================================

Change the desktop background to a random image (from paths you specify) when you log in. Works on Gtk-3-based Linux desktops (Cinnamon, Unity, ...) and Windows XP to 7.

Originally written to teach myself Qt back when Windows didn't have this sort of thing built in; now mostly redundant, but I still use it.


Build
-----

Should work with Qt 4 or 5:

    qmake
    make

Or use Qt Creator. Binaries are placed in `bin`; you can move them to anywhere you like.


Run
---

Run `yabr-config` first to set everything up. You can then change the background manually through the configuration program, or by running `yabr`. Check the box in the configuration program to make it change the background automatically at startup.

Launchers for Linux desktops are in `launchers`; copy the two files to your desktop. They assume `yabr` and `yabr-config` are on your PATH.


License
-------

Copyright (C) 2009 -- 2021 Callie Cameron

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

The icon `yabr-config/alert.png` is a modified version of http://findicons.com/icon/169459/alert, also released under the GPL.
