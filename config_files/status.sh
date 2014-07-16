#!/bin/sh

export amixstatus=$(amixer sget Master| tail -1 | awk '{ print $6 }' | tr -d '[]%')
export amixvolume=$(amixer sget Master| tail -1 | awk '{ print $4 }' | amixer sget Master| tail -1 | awk '{ print $4 }' | tr -d '[]%')

echo '{"version":1}'
echo '['
echo '[],'
exec conky -c $HOME/.conky/conky2
