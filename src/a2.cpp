#include "a2.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>

using namespace std;


/* 2 functions you've already implemented in assignment 1 */

// Change the brightness of the image
// const FloatImage & means a reference to im will get passed to you, 
// but the compiler won't let you modify it
FloatImage brightness(const FloatImage &im, float factor)
{
	// Create output FloatImage
	// Modify image brightness
	// return output;
	FloatImage output(im);	// make a copy of the image to modify

	// Loop through the image and multiply each pixel by factor
	output *= factor;
	return output;
}

FloatImage contrast(const FloatImage &im, float factor, float midpoint)
{
	// Create output FloatImage
	// Modify image contrast
	// return output;
	FloatImage output(im);	// make a copy of the image to modify

	// Loop through the image and apply contrast function
	output = factor * (output - midpoint) + midpoint;
	return output;
}


/*New tasks in assignment 2 */

FloatImage changeGamma(const FloatImage &im, float old_gamma, float new_gamma)
{
	// create an appropriately sized output FloatImage
	FloatImage output(im);

	// Figure out what power to take the values of im, to get the values of output
	for (auto i = 0; i < output.size(); ++i) {
		// convert from x^old_gamma to x
		// note that output(i) == x^old_gamma
		auto x = output(i) * pow(output(i), pow((old_gamma), -1) - 1);

		// convert from x to x^new_gamma
		output(i) = pow(x, new_gamma);
	}
	return output;
}

// Change the exposure of the image. This is different than brightness because
// it means you multiply an image's intensity in the linear domain.
FloatImage exposure(const FloatImage &im, float factor)
{
	// create an appropriately sized output FloatImage
	FloatImage output(im);

	float gamma = 1/2.2; // assumed gamma value

	// gamma decode, multiply each pixel by factor, then gamma encode
	output = changeGamma(output, gamma, 1);
	output *= factor;
	output = changeGamma(output, 1, gamma);

	return output;
}

FloatImage color2gray(const FloatImage &im, const vector<float> &weights)
{
	if ((int) weights.size() != im.channels())
		throw MismatchedDimensionsException();

	// Initialize a new image of the same size but only one channel for intensity)
	FloatImage output(im.width(), im.height(), 1);

	// Convert to grayscale by performing a weighted average across RGB channels
	// based on perceived brightness
	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++y) {
			output(x,y,0) += weights[0] * im(x,y,0) + 
							 weights[1] * im(x,y,1) + 
							 weights[2] * im(x,y,2);
		}
	}
	return output;
}

// For this function, we want two outputs, a single channel luminance image
// and a three channel chrominance image. Return them in a vector with luminance first
vector<FloatImage> lumiChromi(const FloatImage &im)
{
	vector<FloatImage> output;

	// Create luminance and chrominance images
	FloatImage lumi = color2gray(im);
	FloatImage chromi = im/(lumi + 1e-9);

	// push the luminance and chrominance images onto the vector
	output.push_back(lumi);
	output.push_back(chromi);

	return output;
}

// go from a luminance/chrominance images back to an rgb image
FloatImage lumiChromi2rgb(const FloatImage &lumi, const FloatImage &chromi)
{
	FloatImage output;
	
	// combine the luminance and chrominance images into an rgb image
	output = chromi * lumi;
	return output;
}

// Modify brightness then contrast
FloatImage brightnessContrastLumi(const FloatImage &im, float brightF, float contrastF, float midpoint)
{
	// Create output image of appropriate size
	FloatImage output(im);

	// Decompose the image into luminance and chrominance 
	vector<FloatImage> vector = lumiChromi(im);
	FloatImage lumi = vector[0];
	FloatImage chromi = vector[1];

	// Modify brightness, then contrast of luminance image
	FloatImage new_lumi = brightness(lumi, brightF);
	new_lumi = contrast(new_lumi, contrastF, midpoint);

	// Combine modified lumi and chromi into an RGB image
	output = lumiChromi2rgb(new_lumi, chromi);

	return output;
}

FloatImage rgb2yuv(const FloatImage &im)
{
	// Create output image of appropriate size
	FloatImage output(im);

	// Loop over image pixels to change colorspace
	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++ y) {

			// Get the RGB encoded value for each channel
			auto R = output(x,y,0);
			auto G = output(x,y,1);
			auto B = output(x,y,2);

			// RGB to YUV matrix multiplication
			auto Y = 0.299*R + 0.587*G + 0.114*B;
			auto U = -0.147*R - 0.289*G + 0.436*B;
			auto V = 0.615*R - 0.515*G - 0.100*B;

			// Set each channel to the corresponding YUV encoded value
			output(x,y,0) = Y;
			output(x,y,1) = U;
			output(x,y,2) = V;
		}
	}
	return output;
}

FloatImage yuv2rgb(const FloatImage &im)
{
	// Create output image of appropriate size
	FloatImage output(im);

	// Loop over image pixels to change colorspace
	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++y) {

			// Get the YUV encoded value for each channel
			auto Y = output(x,y,0);
			auto U = output(x,y,1);
			auto V = output(x,y,2);

			// YUV to RGB matrix multiplication
			auto R = 1*Y + 0*U + 1.14*V;
			auto G = 1*Y - 0.395*U - 0.581*V;
			auto B = 1*Y + 2.032*U + 0*V;

			// Set each channel to the corresponding RGB encoded value
			output(x,y,0) = R;
			output(x,y,1) = G;
			output(x,y,2) = B;
		}
	}
	return output;
}

FloatImage saturate(const FloatImage &im, float factor)
{
	// Create output image of appropriate size
	FloatImage output(im);

	// convert to YUV colorspace
	FloatImage yuv_im = rgb2yuv(im);

	// Scale the U and V channels by factor to saturate
	for (auto x = 0; x < yuv_im.width(); ++x) {
		for (auto y = 0; y < yuv_im.height(); ++y) {
			auto U = yuv_im(x,y,1);
			auto V = yuv_im(x,y,2);

			U *= factor;
			V *= factor;

			yuv_im(x,y,1) = U;
			yuv_im(x,y,2) = V;
		}
	}

	// Convert back to RGB colorspace
	output = yuv2rgb(yuv_im);
	return output;
}

// Return two images in a C++ vector
vector<FloatImage> spanish(const FloatImage &im)
{
	// Remember to create the output images and the output vector
	vector<FloatImage> output;
	FloatImage im1;
	FloatImage im2;

	// Do all the required processing
	// first image has constant luminance (Y) and negative chrominance (U and V)
	im1 = rgb2yuv(im);
	for (auto x = 0; x < im1.width(); ++x) {
		for (auto y = 0; y < im1.height(); ++y) {
			im1(x,y,0) = 0.5;	// set Y channel to 0.5
			im1(x,y,1) *= -1;	// make U channel negative
			im1(x,y,2) *= -1;	// make V channel negative
		}
	}
	// convert back to RGB
	im1 = yuv2rgb(im1);

	// Draw a black dot at the center of the image to help the viewer focus
	int cx = floor(im1.width()/2);
	int cy = floor(im.height()/2);
	int r = im.width()/300;

	for (auto z = 0; z < im1.channels(); ++z) {
		for (auto x = cx - r; x < cx + r; ++x) {
			for (auto y = cy - r; y < cy + r; ++ y) {
				im1(x, y, z) = 0;
			}
		}
	}

	// second image is grayscale of original (work in YUV encoding and set U & V to 0)
	im2 = rgb2yuv(im);
	for (auto x = 0; x < im2.width(); ++x) {
		for (auto y = 0; y < im2.height(); ++y) {
			im2(x,y,1) = 0;	// set U channel to 0
			im2(x,y,2) = 0;	// set V channel to 0
		}
	}

	// black dot
	for (auto z = 0; z < im2.channels(); ++z) {
		for (auto x = cx - r; x < cx + r; ++x) {
			for (auto y = cy - r; y < cy + r; ++ y) {
				im2(x, y, z) = 0;
			}
		}
	}

	// convert back to RGB
	im2 = yuv2rgb(im2);

	// Push the images onto the vector
	output.push_back(im1);
	output.push_back(im2);

	// Return the vector
	return output;
}

// White balances an image using the gray world assumption
FloatImage grayworld(const FloatImage &im)
{
	// Your code goes here
	FloatImage output(im); 

	// Innitialize sums for each channel value; 
	// we want to find their individual means in the image
	float sum_r = 0;
	float sum_g = 0;
	float sum_b = 0;
	int num_pixels = output.width() * output.height();

	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++y) {
			sum_r += output(x,y,0); // sum the red values
			sum_g += output(x,y,1); // sum the green values
			sum_b += output(x,y,2); // sum the blue values
		}
	}
	
	float mean_r = sum_r / num_pixels;
	float mean_g = sum_g / num_pixels;
	float mean_b = sum_b / num_pixels;

	// Get the factor for each channel to multiply by;
	// this factor makes the mean of each channel the same
	float factor_r = mean_g / mean_r;
	float factor_g = mean_g / mean_g;
	float factor_b = mean_g / mean_b;

	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++y) {
			output(x,y,0) *= factor_r;
			output(x,y,1) *= factor_g;
			output(x,y,2) *= factor_b;
		}
	}
	return output;
}


// Histograms

// Stretches the pixel values of channel c of im so that the minimum value maps to 0,
// and the maximum value maps to 1
void autoLevels(FloatImage &im, int c)
{
	// stretch pixel values of channel c to fill 0..1 range
	// you may find FloatImage::min and FloatImage::max useful

	// get the min and max values of the channel, calculate the range
	auto min = im.min(c);
	auto max = im.max(c);
	auto range = max - min;

	// perform (value - min) / range on each pixel to stretch it to fill 0...1
	for (auto x = 0; x < im.width(); ++x) {
		for (auto y = 0; y < im.height(); ++y) {
			im(x,y,c) = (im(x,y,c) - min) / range;
		}
	}
}

// initialize a histogram with numBins bins from the c-th channel of im
Histogram::Histogram(const FloatImage &im, int c, int numBins) :
	m_pdf(numBins, 0.0f), m_cdf(numBins, 0.0f)
{
	// bin size is found by subdividing 1 by the number of bins
	auto binSize = 1.0f/numBins;

	// populate m_pdf with histogram values
	for (auto x = 0; x < im.width(); ++x) {
		for (auto y = 0; y < im.height(); ++y) {
			int bin_index = floor(im(x,y,c)/binSize);
			m_pdf[bin_index]++;
		}
	}

	// normalize the pdf so the bin values sum to 1
	int totalPixels = im.width() * im.height();
	for (auto i = 0; i < numBins; ++i) {
		m_pdf[i] /= totalPixels;
	}

	// Grad/extra credit: populate m_cdf with running sum of m_pdf
	m_cdf[0] = m_pdf[0];
	for (auto i = 1; i < m_pdf.size(); ++i) {
		m_cdf[i] = m_pdf[i] + m_cdf[i-1];
	}
}

// return the histogram bin that value falls within
int Histogram::inverseCDF(double value) const
{
	for (float i = 0; i < numBins(); ++i) {
		if (value > cdf(i)) {
			if (value < cdf(i+1)) {
				float bin = i;
				return bin;
			}
		}
	}
}

// Produce a numBins() x 100 x 3 image containing a visualization of the
// red, green and blue histogram pdfs
FloatImage visualizeRGBHistogram(const Histogram &histR,
                                 const Histogram &histG,
                                 const Histogram &histB)
{
	assert(histR.numBins() == histG.numBins() && histR.numBins() == histB.numBins());

	// create an image of appropriate size
	FloatImage im(histR.numBins(), 100, 3);

	// find max value across all channels to set appropriate scale
	float max = 0;
	for (auto i = 0; i < histR.numBins(); ++i) {
		if (histR.pdf(i) > max) max = histR.pdf(i);
	}
	for (auto i = 0; i < histG.numBins(); ++i) {
		if (histG.pdf(i) > max) max = histG.pdf(i);
	}
	for (auto i = 0; i < histB.numBins(); ++i) {
		if (histB.pdf(i) > max) max = histB.pdf(i);
	}

	max *= 1.01; // bug fix: bump the max a bit to counteract floating point errors
	float scale = 100 / max; // scales the histogram to fill the entire height of image
	 
	// populate im with RGB pdf calues
	for (auto x = 0; x < histR.numBins(); ++x) {
		for (auto y = 99; y > 99 - histR.pdf(x)*scale; --y) { 	// red
			im(x,y,0) = 1;
		}
		for (auto y = 99; y > 99 - histG.pdf(x)*scale; --y) { 	// green
			im(x,y,1) = 1;
		}
		for (auto y = 99; y > 99 - histB.pdf(x)*scale; --y) {	// blue
			im(x,y,2) = 1;
		}
	}

	// Draw the CDF curve (averaged across all channels)
	for (auto x = 0; x < histR.numBins(); ++x) {
		float cum_avg = ( histR.cdf(x) + histG.cdf(x) + histB.cdf(x) ) / 3;
		auto y = 99 - cum_avg*99;
		// draw a curve of gray points
		im(x,y,0) = .5;
		im(x,y,1) = .5;
		im(x,y,2) = .5;
	}
	return im;
}

// Return a FloatImage which is the result of applying histogram equalization to im
FloatImage equalizeRGBHistograms(const FloatImage &im)
{
	int numLevels = 256;

	// create a copy of input image
	FloatImage output = im;

	// create RGB histograms for the image
	Histogram histR = Histogram(output, 0);
	Histogram histG = Histogram(output, 1);
	Histogram histB = Histogram(output, 2);

	// apply histogram equalization to each channel
	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++y) {
			
			// map old intensity to the cdf value of the intensity's bin
			float old_R_intensity = output(x,y,0);
			float new_R_intensity = histR.cdf(old_R_intensity * numLevels);

			float old_G_intensity = output(x,y,1);
			float new_G_intensity = histG.cdf(old_G_intensity * numLevels);

			float old_B_intensity = output(x,y,2);
			float new_B_intensity = histB.cdf(old_B_intensity * numLevels);

			output(x,y,0) = new_R_intensity;
			output(x,y,1) = new_G_intensity;
			output(x,y,2) = new_B_intensity;
		}
	}

	return output;
}

// Return a FloatImage which is the result of transfering the histogram of F2 onto the image data in F1
FloatImage matchRGBHistograms(const FloatImage &F1, const FloatImage &F2)
{
	int numBins = 256;

	FloatImage output = F1;

	// perform histogram matching
	Histogram hist1R = Histogram(output, 0);
	Histogram hist1G = Histogram(output, 1);
	Histogram hist1B = Histogram(output, 2);

	Histogram hist2R = Histogram(F2, 0);
	Histogram hist2G = Histogram(F2, 1);
	Histogram hist2B = Histogram(F2, 2);

	for (auto x = 0; x < output.width(); ++x) {
		for (auto y = 0; y < output.height(); ++y) {

			// map old intensity to be the corresponding bin of the target image's
			// cdf value at that intensity
			float old_1R_intensity = output(x,y,0);
			float old_1R_cdf_val = hist1R.cdf(old_1R_intensity * (numBins-1));
			int new_1R_bin = hist2R.inverseCDF(old_1R_cdf_val);
			float new_1R_intensity = (float) new_1R_bin / numBins;

			float old_1G_intensity = output(x,y,1);
			float old_1G_cdf_val = hist1G.cdf(old_1G_intensity * (numBins-1));
			int new_1G_bin = hist2G.inverseCDF(old_1G_cdf_val);
			float new_1G_intensity = (float) new_1G_bin / numBins;

			float old_1B_intensity = output(x,y,2);
			float old_1B_cdf_val = hist1B.cdf(old_1B_intensity * (numBins-1));
			int new_1B_bin = hist2B.inverseCDF(old_1B_cdf_val);
			float new_1B_intensity = (float) new_1B_bin / numBins;
			if (new_1B_intensity > 1) new_1B_intensity = 0.99;

			output(x,y,0) = new_1R_intensity;
			output(x,y,1) = new_1G_intensity;
			output(x,y,2) = new_1B_intensity;
		}
	}
	return output;
}

