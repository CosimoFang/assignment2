CS5330 Project 2 Content-based Image Retrieval
Minghao Liu

os win10
ide vs studio

Khoury wiki:
https://wiki.khoury.northeastern.edu/display/~mohsen/CS5330+PROJECT+2++Content-based+Image+Retrieval

filter.h/filter.cpp include filtering and generate histogram and calulated difference.
vidDisplay.cpp: the mainfunction of the program, read img, read img in the directory, sort and out put

how to use:
1 use cd to the dir and use cmd to run the exe file with command line input.
cmd : (name of program).exe (target img name) (source of database) (bin number) (matching method) (top N closed img)

you can also run exe withtout parameter to run the coded setup, this can be changed in vidDisplay.cpp line 78
matching method:

"baseline" req1 using center 9x9 for sum of difference sqr
"single_histogram" req 2 using whole img and red and green channel only for histogram difference
"multi_histogram"  req 3 using whole img and center 141x141 pixel 3 color channel for histogram difference
"texture_matching" req 4 using whole img and magnituded img 3 color channel for histogram difference
"custom_matching_1" req5 using whole img, magnituded img 3 color channel,center 200x200 pixel, and center red pixel only for histogram difference
"custom_matching_2" req5 using whole img, magnituded img 3 color channel,center 200x200 pixel, and center green pixel for histogram difference
