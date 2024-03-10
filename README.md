# HyprSwitchr

An urgent-first/most-recent window switcher inspired by [swayr](https://sr.ht/~tsdh/swayr/)

## Install

Currently tested and working with `v0.36.0` commit `1c460e98f870676b15871fe4e5bfeb1a32a3d6d8`  
`hyprpm add https://github.com/pyt0xic/hyprswitchr --verbose`

## Usage

1. Enable with `hyprpm enable hyprswitchr`
2. Make sure you have `exec-once=hyprpm reload -n` in your `hyprland.conf`
3. Switch to urgent or last used window `hyprctl dispatch switch-to-urgent-or-last`
4. Switch to matching, urgent or last used window `hyprctl dispatch switch-to-matching-or-urgent-or-last '^(.*kitty.*)'`
5. Get the window list `hyprctl windowlist` or in JSON format `hyprctl windowlist -j`
