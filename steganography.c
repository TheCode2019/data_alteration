#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 8
#define MAX_MSG_SIZE 256

void hide_message(char *image_path, char *message_path, char *output_path) {
    // Open the image file
    FILE *image_file = fopen(image_path, "rb");
    if (image_file == NULL) {
        printf("Error: could not open image file\n");
        exit(1);
    }

    // Open the message file
    FILE *message_file = fopen(message_path, "rb");
    if (message_file == NULL) {
        printf("Error: could not open message file\n");
        exit(1);
    }

    // Open the output file
    FILE *output_file = fopen(output_path, "wb");
    if (output_file == NULL) {
        printf("Error: could not create output file\n");
        exit(1);
    }

    // Read the image header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image_file);
    fwrite(header, sizeof(unsigned char), 54, output_file);

    // Read the message data
    unsigned char message[MAX_MSG_SIZE];
    fread(message, sizeof(unsigned char), MAX_MSG_SIZE, message_file);

    // Embed the message into the image
    unsigned char block[BLOCK_SIZE];
    int msg_index = 0;
    while (fread(block, sizeof(unsigned char), BLOCK_SIZE, image_file) == BLOCK_SIZE) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            // Embed one bit of the message into the least significant bit of each pixel value
            if (msg_index < MAX_MSG_SIZE * 8) {
                block[i] = (block[i] & 0xFE) | ((message[msg_index / 8] >> (7 - msg_index % 8)) & 0x01);
                msg_index++;
            }
        }
        fwrite(block, sizeof(unsigned char), BLOCK_SIZE, output_file);
    }

    // Copy the remaining bytes of the image file
    int remaining_bytes = ftell(image_file) - ftell(output_file);
    unsigned char remaining_data[remaining_bytes];
    fread(remaining_data, sizeof(unsigned char), remaining_bytes, image_file);
    fwrite(remaining_data, sizeof(unsigned char), remaining_bytes, output_file);

    // Close the files
    fclose(image_file);
    fclose(message_file);
    fclose(output_file);

    printf("Message hidden successfully!\n");
}

void extract_message(char *stego_image_path, char *output_path) {
    // Open the stego-image file
    FILE *stego_image_file = fopen(stego_image_path, "rb");
    if (stego_image_file == NULL) {
        printf("Error: could not open stego-image file\n");
        exit(1);
    }

    // Open the output file
    FILE *output_file = fopen(output_path, "wb");
    if (output_file == NULL) {
        printf("Error: could not create output file\n");
