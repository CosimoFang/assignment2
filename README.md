# Project 2 Content-based Image Retrieval

Hongchao Fang

os win10
ide vs studio

filter.h/filter.cpp include filtering and generate histogram and calulated difference.
vidDisplay.cpp: the mainfunction of the program, read img, read img in the directory, sort and out put

If you want to redo the test, just run:
```
imgDisplay.exe (target img name) (source of database) (bin number) (matching method) (top N closed img)
```

"baseline" req1 using center 9x9 for sum of difference sqr
"single_histogram" req 2 using whole img and red and green channel only for histogram difference
"multi_histogram"  req 3 using whole img and center 141x141 pixel 3 color channel for histogram difference
"texture_matching" req 4 using whole img and magnituded img 3 color channel for histogram difference
"custom_matching_1" req5 using whole img, magnituded img 3 color channel,center 200x200 pixel, and center red pixel only for histogram difference
"custom_matching_2" req5 using whole img, magnituded img 3 color channel,center 200x200 pixel, and center green pixel for histogram difference
