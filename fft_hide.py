import cv2
import numpy as np
from scipy.fftpack import fft2, ifft2


def hide_data_in_image(img_path, data):
    # Load the image and convert to grayscale
    img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)

    # Get the Fourier Transform of the image
    f = fft2(img)

    # Get the shape of the image and data
    img_shape = img.shape
    data_len = len(data)

    # Calculate the number of bits needed to hide the data
    bits_needed = img_shape[0] * img_shape[1] * 8

    # Check if the data can fit in the image
    if data_len * 8 > bits_needed:
        print("Error: Data is too large to hide in the image.")
        return

    # Convert the data to binary
    binary_data = ''.join(format(ord(i), '08b') for i in data)

    # Append the binary data with null character to indicate the end of the data
    binary_data += '00000000'

    # Get the length of the binary data
    binary_data_len = len(binary_data)

    # Reshape the Fourier-transformed image data to 1D array
    f_1d = np.reshape(f, -1)

    # Hide the binary data in the least significant bit of the Fourier-transformed image data
    for i in range(binary_data_len):
        bit = binary_data[i]
        f_1d[i] = f_1d[i].real + int(bit)

    # Reshape the 1D Fourier-transformed image data to original shape
    f_hidden = np.reshape(f_1d, img_shape)

    # Get the inverse Fourier Transform of the hidden image data
    img_hidden = np.abs(ifft2(f_hidden)).astype(np.uint8)

    return img_hidden