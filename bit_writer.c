#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bit_writer.h"

BitWriter open_bit_writer(const char* path) {
	return (BitWriter) {.file = fopen(path , "w") , .current_byte = 0, .num_bits_left = 8};
}

void write_bits(BitWriter* a_writer, uint8_t bits, uint8_t num_bits_to_write) {
	assert(num_bits_to_write <= 8 && num_bits_to_write >= 0);
	assert(a_writer -> num_bits_left >= 1 && a_writer -> num_bits_left <= 8);	
	
	//printf("%d   %d\n" , a_writer -> num_bits_left , num_bits_to_write);

	int secondChunk = a_writer -> num_bits_left;

	if (num_bits_to_write >= 0) {
		if (num_bits_to_write < (a_writer -> num_bits_left)) {
			bits = bits << (8 - num_bits_to_write);
			bits = bits >> (8 - (a_writer -> num_bits_left));

			a_writer -> current_byte = a_writer -> current_byte ^ bits; 
			a_writer -> num_bits_left -= num_bits_to_write;
		}
		else {
			bits = bits << (8 - num_bits_to_write);
			bits = bits >> (8 - (a_writer -> num_bits_left));

			a_writer -> current_byte = a_writer -> current_byte ^ bits; 
			a_writer -> num_bits_left -= num_bits_to_write;

			flush_bit_writer(a_writer);			

			bits = bits << (8 - (num_bits_to_write - secondChunk));
			bits = bits >> (8 - (a_writer -> num_bits_left));

			a_writer -> current_byte = a_writer -> current_byte ^ bits; 
			a_writer -> num_bits_left -= (num_bits_to_write - secondChunk);
		}
	}

	assert(a_writer -> num_bits_left >= 1 && a_writer -> num_bits_left <= 8);
}

void flush_bit_writer(BitWriter* a_writer) {
	if((a_writer -> num_bits_left) != 8){
		fwrite(&(a_writer -> current_byte), sizeof(a_writer -> current_byte) , 1, (a_writer -> file));
	}
	a_writer -> file = (a_writer -> file);
	a_writer -> current_byte = 0;
	a_writer -> num_bits_left = 8;

}

void close_bit_writer(BitWriter* a_writer) {
	flush_bit_writer(a_writer);
	fclose(a_writer -> file);
	a_writer -> file = NULL;
}

#define __BIT_WRITER_C_V1__
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab colorcolumn=96: */
