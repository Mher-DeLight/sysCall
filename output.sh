x=3.2
if [[ $(echo "${x}>=4" | bc -l) = 1 ]]; then
echo greater or equal
fi
if [[ $(echo "${x}<4" | bc -l) = 1 ]]; then
echo less
fi
