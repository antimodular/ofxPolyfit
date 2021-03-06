# Introduction
Fit data between any 2 datasets of arbitrary dimensionality

Just a quick note to introduce ofxPolyfit. It’s an openFrameworks extension which allows you to correlate between any 2 numerical datasets of the same or different dimensionality. e.g. a stereo cam is 2x2D, and you want to get 3D, so you could use ofxPolyfit to create your 4D->3D correlation.

## Features
* MD input, ND output
* templated (choose double or float depending on accuracy / speed requirementes)
* Object oriented datasets (see pfitDataSet<T> and pfitDataPoint<T>)
* Simple ransac implementation (use ::RANSAC(..) instead of ::correlate(..) ) - may come in and out of usage


## TODO
* convert bases to be more flexible / OO / communicable
* create coeffs class that can save / load, associates with bases