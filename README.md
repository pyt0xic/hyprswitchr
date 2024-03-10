# HyprSwitchr

An urgent-first/most-recent window switcher inspired by [swayr](https://sr.ht/~tsdh/swayr/)

## Install

Currently tested and working with `v0.36.0` commit `1c460e98f870676b15871fe4e5bfeb1a32a3d6d8`  
`hyprpm add https://github.com/pyt0xic/hyprswitchr --verbose`

## Usage

1. Enable with `hyprpm enable hyprswitchr`
2. Make sure you have `exec-once=hyprpm reload -n` in your `hyprland.conf`
3. Switch to urgent or most-recently-used window `hyprctl dispatch switch-to-urgent-or-prev`
4. Switch to urgent or least-recently-used window `hyprctl dispatch switch-to-urgent-or-next`
5. Switch to matching, urgent or most-recently-used window `hyprctl dispatch switch-to-matching-urgent-or-prev '^(.*kitty.*)'`
6. Switch to matching, urgent or least-recently-used window `hyprctl dispatch switch-to-matching-urgent-or-next '^(.*kitty.*)'`
7. Get the window list in most-recently-used order `hyprctl windowlist` or in JSON format `hyprctl windowlist -j`
8. Get the window list in least-recently-used order `hyprctl windowlist-reverse` or in JSON format `hyprctl windowlist -j`
