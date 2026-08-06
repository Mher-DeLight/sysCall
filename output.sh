#!/bin/bash
# This is an auto-generated Bash file. Some sections may appear cryptic while others may be verbose.
# Some elements may also not be stylized or formatted for readability.
# If present, it is better to read the .scl file that generated this script, as it would be more readable.
# Generated with sysCall. https://www.github.com/Mher-DeLight/SysCAll

x=3.2
if [[ $(echo "${x}>=4" | bc -l) = 1 ]]; then
x=$(echo "${x}+1" | bc -l)
fi
if [[ $(echo "${x}<4" | bc -l) = 1 ]]; then
x=$(echo "${x}-1" | bc -l)
fi
echo $x
