#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>

#define NB 6	// No. of Branches: 4 for Sparkle256, 6 for Sparkle384, 8 for Sparkle512
#define NS 1	// No. of Step

#define MAX_BRANCHES 8

#define ROT32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define ELL32(x) (ROT32(((x) ^ ((x) << 16)), 16))

#define ROT16(x, n) (((x) >> (n)) | ((x) << (16 - (n))))
#define ELL16(x) (ROT16(((x) ^ ((x) << 8)), 8))

#define ROT12(x, n) ((((x) >> (n)) & 0x0FFF) | (((x) << (12 - (n))) & 0x0FFF))

#define ROT8(x, n) (((x) >> (n)) | ((x) << (8 - (n))))
#define ELL8(x) (ROT8(((x) ^ ((x) << 4)), 4))

// Round constants
static const uint32_t RCON[MAX_BRANCHES] = { \
	0xB7E15162, 0xBF715880, 0x38B4DA56, 0x324E7738, \
	0xBB1185EB, 0x4F7C7B57, 0xCFBFA1C8, 0xC2B3293D \
};

// Alzette 64-bit input
void alzette64(uint32_t* state, int nbranch)
{
	uint32_t rc;

	rc = RCON[nbranch >> 1];
	state[0] += ROT32(state[1], 31);
	state[1] ^= ROT32(state[0], 24);
	state[0] ^= rc;
	state[0] += ROT32(state[1], 17);
	state[1] ^= ROT32(state[0], 17);
	state[0] ^= rc;
	state[0] += state[1];
	state[1] ^= ROT32(state[0], 31);
	state[0] ^= rc;
	state[0] += ROT32(state[1], 24);
	state[1] ^= ROT32(state[0], 16);
	state[0] ^= rc;
}

// Alzette 32-bit input
void alzette32(uint16_t* state, int nbranch)
{
	uint16_t rc;

	rc = RCON[nbranch >> 1];
	state[0] += ROT16(state[1], 15);
	state[1] ^= ROT16(state[0], 12);
	state[0] ^= rc;
	state[0] += ROT16(state[1], 9);
	state[1] ^= ROT16(state[0], 9);
	state[0] ^= rc;
	state[0] += state[1];
	state[1] ^= ROT16(state[0], 15);
	state[0] ^= rc;
	state[0] += ROT16(state[1], 12);
	state[1] ^= ROT16(state[0], 8);
	state[0] ^= rc;
}

// Alzette 24-bit input
void alzette24(uint16_t* state, int nbranch)
{
	uint16_t rc;

	rc = RCON[nbranch >> 1] & 0x0FFF;
	state[0] += ROT12(state[1], 11);
	state[0] &= 0x0FFF;
	state[1] ^= ROT12(state[0], 9);
	state[0] ^= rc;
	state[0] += ROT12(state[1], 7);
	state[0] &= 0x0FFF;
	state[1] ^= ROT12(state[0], 7);
	state[0] ^= rc;
	state[0] += state[1];
	state[0] &= 0x0FFF;
	state[1] ^= ROT12(state[0], 11);
	state[0] ^= rc;
	state[0] += ROT12(state[1], 9);
	state[0] &= 0x0FFF;
	state[1] ^= ROT12(state[0], 6);
	state[0] ^= rc;
}

// Alzette 16-bit input
void alzette16(uint8_t* state, int nbranch)
{
	uint8_t rc;

	rc = RCON[nbranch >> 1];
	state[0] += ROT8(state[1], 7);
	state[1] ^= ROT8(state[0], 6);
	state[0] ^= rc;
	state[0] += ROT8(state[1], 5);
	state[1] ^= ROT8(state[0], 5);
	state[0] ^= rc;
	state[0] += state[1];
	state[1] ^= ROT8(state[0], 7);
	state[0] ^= rc;
	state[0] += ROT8(state[1], 6);
	state[1] ^= ROT8(state[0], 4);
	state[0] ^= rc;
}

int alzette16_p()
{
	uint8_t input1[2] = { 0x00, };
	uint8_t input2[2] = { 0x00, };
	uint8_t in_diff[2] = { 0x00, };
	uint8_t out_diff[2] = { 0x00, };

	int i, j;

	int cnt = 0;

	for (i = 0; i < 2; i++) {
		in_diff[i] = (uint8_t)rand();
		out_diff[i] = (uint8_t)rand();
	}

	for (i = 0; i <= 0xFF; i++)
	{
		for (j = 0; j <= 0xFF; j++)
		{
			input1[0] = i;
			input1[1] = j;

			input2[0] = input1[0] ^ in_diff[0];
			input2[1] = input1[1] ^ in_diff[1];

			alzette16(input1, 1);
			alzette16(input2, 1);

			if (out_diff[0] == (input1[0] ^ input2[0]))
			{
				if (out_diff[1] == (input1[1] ^ input2[1]))
				{
					cnt++;
					return cnt;
				}
			}
		}
	}

	return 0;
}

void exp_alzette16_p(int times)
{
	clock_t timer;
	double time_taken;

	int ret = 0;
	int i = 0;
	int cnt = 0;

	printf("Success ratio of reduced Alzette (b=16, p):\n");

	timer = clock();

	for (i = 0; i < times; i++)
	{
		printf("%d\r", i);
		ret = alzette16_p();

		if (ret > 0) cnt++;
	}

	timer = clock() - timer;
	time_taken = ((double)timer) / CLOCKS_PER_SEC;

	printf("%d/%d (%f sec)\n", cnt, times, time_taken);
}

int alzette16_q()
{
	uint8_t input1[2] = { 0x00, };
	uint8_t input2[2] = { 0x00, };
	uint8_t in_diff[2] = { 0x00, };
	uint8_t out_diff[2] = { 0x00, };
	uint8_t k1[2] = { 0x00, };
	uint8_t k2[2] = { 0x00, };

	int i, j;

	int cnt = 0;

	for (i = 0; i < 2; i++) {
		in_diff[i] = (uint8_t)rand();
		out_diff[i] = (uint8_t)rand();
		k1[i] = (uint8_t)rand();
		k2[i] = (uint8_t)rand();
	}

	for (i = 0; i <= 0xFF; i++)
	{
		for (j = 0; j <= 0xFF; j++)
		{
			input1[0] = i;
			input1[1] = j;

			input2[0] = input1[0] ^ in_diff[0];
			input2[1] = input1[1] ^ in_diff[1];

			alzette16(input1, 1);
			alzette16(input2, 1);

			input1[0] ^= k1[0];
			input1[1] ^= k1[1];

			input2[0] ^= k2[0];
			input2[1] ^= k2[1];

			alzette16(input1, 2);
			alzette16(input2, 2);

			if (in_diff[0] == (input1[0] ^ input2[0]))
			{
				if (in_diff[1] == (input1[1] ^ input2[1]))
				{
					cnt++;
					return cnt;
				}
			}
		}
	}

	return cnt;
}

void exp_alzette16_q(int times)
{
	clock_t timer;
	double time_taken;

	int ret = 0;
	int i = 0;
	int cnt = 0;

	printf("Success ratio of reduced Alzette (b=16, q):\n");

	timer = clock();

	for (i = 0; i < times; i++)
	{
		printf("%d\r", i);
		ret = alzette16_q();

		if (ret > 0) cnt++;
	}

	timer = clock() - timer;
	time_taken = ((double)timer) / CLOCKS_PER_SEC;

	printf("%d/%d (%f sec)\n", cnt, times, time_taken);
}

int alzette24_p()
{
	uint16_t input1[2] = { 0x0000, };
	uint16_t input2[2] = { 0x0000, };
	uint16_t in_diff[2] = { 0x0000, };
	uint16_t out_diff[2] = { 0x0000, };

	int i, j;

	int cnt = 0;

	for (i = 0; i < 2; i++) {
		in_diff[i] = (uint16_t)rand();
		in_diff[i] &= 0x0FFF;
		out_diff[i] = (uint16_t)rand();
		out_diff[i] &= 0x0FFF;
	}

	for (i = 0; i <= 0x0FFF; i++)
	{
		for (j = 0; j <= 0x0FFF; j++)
		{
			input1[0] = i;
			input1[1] = j;

			input2[0] = input1[0] ^ in_diff[0];
			input2[1] = input1[1] ^ in_diff[1];

			alzette24(input1, 1);
			alzette24(input2, 1);

			if ((out_diff[0] == (input1[0] ^ input2[0])) && (out_diff[1] == (input1[1] ^ input2[1])))
			{
				cnt++;
				return cnt;
			}
		}
	}

	return cnt;
}

exp_alzette24_p(int times)
{
	clock_t timer;
	double time_taken;

	int ret = 0;
	int i = 0;
	int cnt = 0;

	printf("Success ratio of reduced Alzette (b=24, p):\n");

	timer = clock();

	for (i = 0; i < times; i++)
	{
		printf("%d\r", i);
		ret = alzette24_p();

		if (ret > 0) cnt++;
	}

	timer = clock() - timer;
	time_taken = ((double)timer) / CLOCKS_PER_SEC;

	printf("%d/%d (%f sec)\n", cnt, times, time_taken);
}

int alzette24_q()
{
	uint16_t input1[2] = { 0x0000, };
	uint16_t input2[2] = { 0x0000, };
	uint16_t in_diff[2] = { 0x0000, };
	uint16_t out_diff[2] = { 0x0000, };
	uint16_t k1[2] = { 0x0000, };
	uint16_t k2[2] = { 0x0000, };

	int i, j;

	int cnt = 0;

	for (i = 0; i < 2; i++) {
		in_diff[i] = (uint16_t)rand();
		in_diff[i] &= 0x0FFF;
		out_diff[i] = (uint16_t)rand();
		out_diff[i] &= 0x0FFF;
		k1[i] = (uint16_t)rand();
		k1[i] &= 0x0FFF;
		k2[i] = (uint16_t)rand();
		k2[i] &= 0x0FFF;
	}

	for (i = 0; i <= 0x0FFF; i++)
	{
		for (j = 0; j <= 0x0FFF; j++)
		{
			input1[0] = i;
			input1[1] = j;

			input2[0] = input1[0] ^ in_diff[0];
			input2[1] = input1[1] ^ in_diff[1];

			alzette24(input1, 1);
			alzette24(input2, 1);

			input1[0] ^= k1[0];
			input1[1] ^= k1[1];

			input2[0] ^= k2[0];
			input2[1] ^= k2[1];

			alzette24(input1, 2);
			alzette24(input2, 1);

			if ((out_diff[0] == (input1[0] ^ input2[0])) && (out_diff[1] == (input1[1] ^ input2[1])))
			{
				cnt++;
				return cnt;
			}
		}
	}

	return cnt;
}

exp_alzette24_q(int times)
{
	clock_t timer;
	double time_taken;

	int ret = 0;
	int i = 0;
	int cnt = 0;

	printf("Success ratio of reduced Alzette (b=24, q):\n");

	timer = clock();

	for (i = 0; i < times; i++)
	{
		printf("%d\r", i);
		ret = alzette24_q();

		if (ret > 0) cnt++;
	}

	timer = clock() - timer;
	time_taken = ((double)timer) / CLOCKS_PER_SEC;

	printf("%d/%d (%f sec)\n", cnt, times, time_taken);
}

int alzette32_p()
{
	uint16_t input1[2] = { 0x0000, };
	uint16_t input2[2] = { 0x0000, };
	uint16_t in_diff[2] = { 0x0000, };
	uint16_t out_diff[2] = { 0x0000, };

	int i, j;

	int cnt = 0;

	for (i = 0; i < 2; i++) {
		in_diff[i] = (uint16_t)rand();
		out_diff[i] = (uint16_t)rand();
	}

	for (i = 0; i <= 0xFFFF; i++)
	{
		for (j = 0; j <= 0xFFFF; j++)
		{
			input1[0] = i;
			input1[1] = j;

			input2[0] = input1[0] ^ in_diff[0];
			input2[1] = input1[1] ^ in_diff[1];

			alzette32(input1, 1);
			alzette32(input2, 1);

			if ((out_diff[0] == (input1[0] ^ input2[0])) && (out_diff[1] == (input1[1] ^ input2[1])))
			{
				cnt++;
				return cnt;
			}
		}
	}

	return cnt;
}

exp_alzette32_p(int times)
{
	clock_t timer;
	double time_taken;

	int ret = 0;
	int i = 0;
	int cnt = 0;

	printf("Success ratio of reduced Alzette (b=32, p):\n");

	timer = clock();

	for (i = 0; i < times; i++)
	{
		printf("%d\r", i);
		ret = alzette32_p();

		if (ret > 0) cnt++;
	}

	timer = clock() - timer;
	time_taken = ((double)timer) / CLOCKS_PER_SEC;

	printf("%d/%d (%f sec)\n", cnt, times, time_taken);
}

int alzette32_q()
{
	uint16_t input1[2] = { 0x0000, };
	uint16_t input2[2] = { 0x0000, };
	uint16_t in_diff[2] = { 0x0000, };
	uint16_t out_diff[2] = { 0x0000, };
	uint16_t k1[2] = { 0x0000, };
	uint16_t k2[2] = { 0x0000, };

	int i, j;

	int cnt = 0;

	for (i = 0; i < 2; i++) {
		in_diff[i] = (uint16_t)rand();
		out_diff[i] = (uint16_t)rand();
		k1[i] = (uint16_t)rand();
		k2[i] = (uint16_t)rand();
	}

	for (i = 0; i <= 0xFFFF; i++)
	{
		for (j = 0; j <= 0xFFFF; j++)
		{
			input1[0] = i;
			input1[1] = j;

			input2[0] = input1[0] ^ in_diff[0];
			input2[1] = input1[1] ^ in_diff[1];

			alzette32(input1, 1);
			alzette32(input2, 1);

			input1[0] ^= k1[0];
			input1[1] ^= k1[1];

			input2[0] ^= k2[0];
			input2[1] ^= k2[1];

			alzette32(input1, 2);
			alzette32(input2, 1);

			if ((out_diff[0] == (input1[0] ^ input2[0])) && (out_diff[1] == (input1[1] ^ input2[1])))
			{
				cnt++;
				return cnt;
			}
		}
	}

	return cnt;
}

exp_alzette32_q(int times)
{
	clock_t timer;
	double time_taken;

	int ret = 0;
	int i = 0;
	int cnt = 0;

	printf("Success ratio of reduced Alzette (b=32, q):\n");

	timer = clock();

	for (i = 0; i < times; i++)
	{
		printf("%d\r", i);
		ret = alzette32_q();

		if (ret > 0) cnt++;
	}

	timer = clock() - timer;
	time_taken = ((double)timer) / CLOCKS_PER_SEC;

	printf("%d/%d (%f sec)\n", cnt, times, time_taken);
}


int main()
{
	time_t t;

	srand((unsigned)time(&t));

	exp_alzette16_p(100000);
	exp_alzette16_q(100000);

	exp_alzette24_p(10000);
	exp_alzette24_q(10000);

	exp_alzette32_p(1000);
	exp_alzette32_q(1000);

	return 0;
}

