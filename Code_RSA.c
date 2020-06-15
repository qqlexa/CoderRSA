#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h> 

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

typedef struct key {
	int exp;
	int n;
} key;

int char_to_number(char symbol) {
	return symbol - 64; // 1 - 26
}

int number_to_char(int symbol) {
	return symbol + 64; // 65 - 90 [A-Z in ASCI]
}

int pow_module(int b, int n, int m) {
	/* https://en.wikipedia.org/wiki/Modular_exponentiation */
	int c = 1;
	int n_ = 0;
	do {
		n_++;
		c *= b;
		c %= m;
	} while (n_ < n);
	printf("\nb^n mod m = %d^%d mod %d  == %d\n", b, n, m, c);
	return c;
}

char char_code_rsa(char letter, int exp, int n) {
	return pow_module(char_to_number(letter), exp, n);
}


char char_decode_rsa(char coded_letter, int exp, int n) {
	return pow_module(char_to_number(coded_letter), exp, n);
}

char int_code_rsa(int letter, int exp, int n) {
	return pow_module(letter, exp, n);
}


char int_decode_rsa(int coded_letter, int exp, int n) {
	return pow_module(coded_letter, exp, n);
}

int main(int argc, char** argv){

	// 31 	65 --> 90,	 1    2    3    4    5    6    7     8    9   10   11    12  13   14  15    16   17   18  19    20  21   22   23   24   25   26
	char alphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	int len_alphabet = 26;

	key open_key, secret_key;

	int letter, new_letter;

	int p, q, n, phi, e;
	int d = 0;

	int x, y;

	FILE* file;
	FILE* file_save;
	
	char name_dat[] = "NAME.dat";
	char name_rsa[] = "NAME.rsa";
	char name_txt[] = "NAME.txt";


	file_save = fopen(name_txt, "w+");
	fprintf(file_save, "Starting\n");
	fclose(file_save);

	file_save = fopen(name_txt, "a+");
	fprintf(file_save, "Read .dat file\n");

	if ((file = fopen(name_dat, "r")) == NULL) {
		scanf("%d", &p);
		scanf("%d", &q);
	}
	else {
		fscanf(file, "%d", &p);
		fscanf(file, "%d", &q);
		fclose(file);
	}

	fprintf(file_save, "p = %d, q = %d\n", p, q);
	fclose(file_save);

	file_save = fopen(name_txt, "a+");

	n = p * q;
	phi = (p - 1) * (q - 1);
	e = 2;

	fprintf(file_save, "n = %d, phi = %d\n", n, phi);
	fprintf(file_save, "e may be:\n");

	while (e < phi) {
		e++;
		if (phi % e != 0) {
			fprintf(file_save, "%d ", e);
		}
	}
	fprintf(file_save, "\n");

	e = 2;
	while (phi % e == 0 && e < phi) {
		e++;
	}

	fprintf(file_save, "e = %d\n", e);

	while ((e * d) % phi != 1){
		d++;
	}
	
	while (d < 0) d += n;
	while (d > n) d -= n;

	fprintf(file_save, "d = %d\n", d);
	fclose(file_save);

	printf("p = %d\nq = %d\n\n", p, q);
	printf("n = %d\n", n);
	printf("phi = %d\n", phi);
	printf("e = %d\n", e);
	printf("d = %d\n", d);

	file_save = fopen(name_txt, "a+");

	open_key.exp = e;
	open_key.n = n;

	secret_key.exp = d;
	secret_key.n = n;
	
	fprintf(file_save, "\nOpen key is {%d:%d}\n", open_key.exp, open_key.n);
	fprintf(file_save, "Secret key is {%d:%d}\n", secret_key.exp, secret_key.n);


	fprintf(file_save, "Decoding\n");
	fprintf(file_save, "Read .rsa file\n");

	printf("\n");
	if ((file = fopen(name_rsa, "r")) == NULL) {
		fputs("Error with reading .rsa file: getting coded message from console char by char\n", file_save);
		printf("Error with reading .rsa file: getting coded message from console char by char\nEnter the code of letter (0 for stop)\n>> ");
		while (scanf("%d", &letter) && letter != 0) {
			new_letter = number_to_char(int_decode_rsa(letter, secret_key.exp, secret_key.n)); // for decoding
			printf("Coded char is %d \n", letter);
			printf("Decoded char is %c %d \n", new_letter, new_letter);
			fprintf(file_save, "%d to %c\n", letter, new_letter);
			printf("\n>> ");
		}
	}
	else {
		fputs("Success reading! Decoding message char by char\n", file_save);
		printf("Success reading! Decoding message char by char\n");
		
		while (feof(file) != 1) {
			fscanf(file, "%d", &letter);
			if (letter == -1) {
				break;
			}
			new_letter = number_to_char(int_decode_rsa(letter, secret_key.exp, secret_key.n)); // for decoding
			printf("Coded char is %d \n", letter);
			printf("Decoded char is %c %d \n", new_letter, new_letter);
			fprintf(file_save, "%d to %c\n", letter, new_letter);
			printf("\n");

		}
		fclose(file);

	}

	fclose(file_save);
	return 0;
}