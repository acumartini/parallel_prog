#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <omp.h>

using namespace cv;

struct pixel {
	double red;
	double green;
	double blue;
	
	pixel(double r, double g, double b) : red(r), green(g), blue(b) {};
};

/*
 * The Prewitt kernels can be applied after a blur to help highlight edges
 * The input image must be gray scale/intensities:
 *     double intensity = (in[in_offset].red + in[in_offset].green + in[in_offset].blue)/3.0;
 * Each kernel must be applied to the blured images separately and then composed:
 *     blurred[i] with prewittX -> Xedges[i]
 *     blurred[i] with prewittY -> Yedges[i]
 *     outIntensity[i] = sqrt(Xedges[i]*Xedges[i] + Yedges[i]*Yedges[i])
 * To turn the out intensity to an out color set each color to the intensity
 *     out[i].red = outIntensity[i]
 *     out[i].green = outIntensity[i]
 *     out[i].blue = outIntensity[i]
 *
 * For more on the Prewitt kernels and edge detection:
 *     http://en.wikipedia.org/wiki/Prewitt_operator
 */
void prewittX_kernel(const int rows, const int cols, double * const kernel) {
	if(rows != 3 || cols !=3) {
		std::cerr << "Bad Prewitt kernel matrix\n";
		return;
	}
	for(int i=0;i<3;i++) {
		kernel[0 + (i*rows)] = -1.0;
		kernel[1 + (i*rows)] = 0.0;
		kernel[2 + (i*rows)] = 1.0;
	}
}

void prewittY_kernel(const int rows, const int cols, double * const kernel) {
        if(rows != 3 || cols !=3) {
                std::cerr << "Bad Prewitt kernel matrix\n";
                return;
        }
        for(int i=0;i<3;i++) {
                kernel[i + (0*rows)] = 1.0;
                kernel[i + (1*rows)] = 0.0;
                kernel[i + (2*rows)] = -1.0;
        }
}

void apply_prewittKs (const int rows, const int cols, pixel * const blurred, pixel * const out)  {
	double Xkernel[3*3], Ykernel[3*3];
    //double outIntensity[rows*cols];
    
    // initialize edge arrays    
	double *Xedges = (double *) malloc(rows * cols * sizeof(double));
	#pragma omp parallel for
    for(int i = 0; i < rows * cols; ++i) {
		Xedges[i] = 0.0;
	}
	double *Yedges = (double *) malloc(rows * cols * sizeof(double));
	#pragma omp parallel for
	for(int i = 0; i < rows * cols; ++i) {
		Yedges[i] = 0.0;
	}

    // initialize prewitt kernels
    prewittX_kernel( 3, 3, Xkernel );
    prewittY_kernel( 3, 3, Ykernel );

    // compute prewitt kernel gradients for each pixel in the blurred array and populate output array in grayscale
	#pragma omp parallel for 
    for(int i = 0; i < rows; ++i) {
	    #pragma omp parallel for
		for(int j = 0; j < cols; ++j) {
			const int out_offset = i + (j*rows);
			// For each pixel in the stencil space, compute the X/Y gradient using the prewitt kernels
			for(int x = i - 1, kx = 0; x <= i + 1; ++x, ++kx) {
				for(int y = j - 1, ky = 0; y <= j + 1; ++y, ++ky) {
					if(x >= 0 && x < rows && y >= 0 && y < cols) {
						const int blurred_offset = x + (y*rows);
						const int k_offset = kx + (ky*3);
                        double intensity = (blurred[blurred_offset].red + blurred[blurred_offset].green + blurred[blurred_offset].blue)/3.0;
						Xedges[out_offset] += Xkernel[k_offset] * intensity;
                        Yedges[out_offset] += Ykernel[k_offset] * intensity;
					}
				}
			}
            // compute euclidean distance between computed prewitt gradients to get a grayscale pixel intensity
            double outIntensity = sqrt( Xedges[out_offset]*Xedges[out_offset] + Yedges[out_offset]*Yedges[out_offset] );
            out[out_offset].red = outIntensity;
            out[out_offset].green = outIntensity;
            out[out_offset].blue = outIntensity;
		}
	}

    // free gradient storage
    free( Xedges );
    free( Yedges );
}

/*
 * The gaussian kernel provides a stencil for blurring images based on a 
 * normal distribution
 */
void gaussian_kernel(const int rows, const int cols, const double stddev, double * const kernel) {
	const double denom = 2.0 * stddev * stddev;
	const double g_denom = M_PI * denom;
	const double g_denom_recip = (1.0/g_denom);
	double sum = 0.0;

    // #pragma omp parallel for
	for(int i = 0; i < rows; ++i) {
	    // #pragma omp parallel for
		for(int j = 0; j < cols; ++j) {
			const double row_dist = i - (rows/2);
			const double col_dist = j - (cols/2);
			const double dist_sq = (row_dist * row_dist) + (col_dist * col_dist);
			const double value = g_denom_recip * exp((-dist_sq)/denom);
			kernel[i + (j*rows)] = value;
			sum += value;
		}
	}
	// Normalize
	const double recip_sum = 1.0 / sum;
	#pragma omp parallel for
	for(int i = 0; i < rows; ++i) {
	    #pragma omp parallel for
		for(int j = 0; j < cols; ++j) {
			kernel[i + (j*rows)] *= recip_sum;
		}		
	}
}

void apply_stencil(const int radius, const double stddev, const int rows, const int cols, pixel * const in, pixel * const out) {
	const int dim = radius*2+1;
	double kernel[dim*dim];
	gaussian_kernel(dim, dim, stddev, kernel);
	
    #pragma omp parallel for
	for(int i = 0; i < rows; ++i) {
	    #pragma omp parallel for
		for(int j = 0; j < cols; ++j) {
			const int out_offset = i + (j*rows);
			// For each pixel, do the stencil
			for(int x = i - radius, kx = 0; x <= i + radius; ++x, ++kx) {
				for(int y = j - radius, ky = 0; y <= j + radius; ++y, ++ky) {
					if(x >= 0 && x < rows && y >= 0 && y < cols) {
						const int in_offset = x + (y*rows);
						const int k_offset = kx + (ky*dim);
						out[out_offset].red   += kernel[k_offset] * in[in_offset].red;
						out[out_offset].green += kernel[k_offset] * in[in_offset].green;
						out[out_offset].blue  += kernel[k_offset] * in[in_offset].blue;
					}
				}
			}
		}
	}
}

int main( int argc, char* argv[] ) {
    double start, end;

	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " imageName\n";
		return 1;
	}    

	// Read image
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if(!image.data ) {
		std::cout <<  "Error opening " << argv[1] << std::endl;
		return -1;
	}
	
    start = omp_get_wtime();

	// Get image into C array of doubles for processing
	const int rows = image.rows;
	const int cols = image.cols;
	pixel * imagePixels = (pixel *) malloc(rows * cols * sizeof(pixel));
	#pragma omp parallel for
	for(int i = 0; i < rows; ++i) {
	    #pragma omp parallel for
		for(int j = 0; j < cols; ++j) {
			Vec3b p = image.at<Vec3b>(i, j);
			imagePixels[i + (j*rows)] = pixel(p[0]/255.0,p[1]/255.0,p[2]/255.0);
		}
	}
	
	// Create output arrays
	pixel * outPixels1 = (pixel *) malloc(rows * cols * sizeof(pixel));
	#pragma omp parallel for
	for(int i = 0; i < rows * cols; ++i) {
		outPixels1[i].red = 0.0;
		outPixels1[i].green = 0.0;
		outPixels1[i].blue = 0.0;
	}
	pixel * outPixels2 = (pixel *) malloc(rows * cols * sizeof(pixel));
	#pragma omp parallel for
	for(int i = 0; i < rows * cols; ++i) {
		outPixels2[i].red = 0.0;
		outPixels2[i].green = 0.0;
		outPixels2[i].blue = 0.0;
	}	

	// Do the stencil
	apply_stencil(3, 32.0, rows, cols, imagePixels, outPixels1);
    
    // Apply grayscale processing
    apply_prewittKs(rows, cols, outPixels1, outPixels2);
	
	// Create an output image (same size as input)
	Mat dest(rows, cols, CV_8UC3);
	// Copy C array back into image for output
	#pragma omp parallel for
	for(int i = 0; i < rows; ++i) {
	    #pragma omp parallel for
		for(int j = 0; j < cols; ++j) {
			const size_t offset = i + (j*rows);
			dest.at<Vec3b>(i, j) = Vec3b(floor(outPixels2[offset].red * 255.0),
										 floor(outPixels2[offset].green * 255.0),
										 floor(outPixels2[offset].blue * 255.0));
		}
	}
	
	imwrite("out.jpg", dest);
	
    end = omp_get_wtime();
    printf( "ptime = %lf\n", end - start );
	
	free(imagePixels);
	free(outPixels1);
	free(outPixels2);
	return 0;
}





