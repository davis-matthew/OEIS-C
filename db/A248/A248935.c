/*
 * A248935  Decimal expansion of 2^4253 - 1, the 19th Mersenne prime
 *          A000668(19).
 * https://oeis.org/A248935
 *
 * Offset:    1281    (a(1281) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = decimal digit of 2^4253 - 1 at position (n-1280), 0 if
 *            n<1281 or beyond the number
 * Range:     u8 n <= 2561, u16 n <= 2561, u32 n <= 2561, u64 n <= 2561,
 *            u128 n <= 2561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper constants for arbitrary-precision arithmetic on decimal 1e9 chunks. */
#define BASE 1000000000u   /* 10^9 */

static unsigned int *big_digits = NULL;   /* 2^4253 - 1 in base BASE */
static size_t big_len = 0;                /* number of BASE chunks */
static char *decimal_str = NULL;          /* decimal string of the number */
static size_t decimal_len = 0;            /* length of decimal_str */

/* Initialize the big integer and decimal representation once. */
static void init_big_number(void)
{
    if (big_digits != NULL)
        return;  /* already initialized */

    /* 2^4253 - 1 has at most 1280 decimal digits, i.e. ceil(1280/9)=143 BASE chunks. */
    big_digits = (unsigned int*)malloc(200 * sizeof(unsigned int));
    big_len = 1;
    big_digits[0] = 1; /* start with value 1 */

    /* Multiply by 2, 4253 times. */
    for (int exp = 0; exp < 4253; ++exp) {
        unsigned long long carry = 0;
        for (size_t i = 0; i < big_len; ++i) {
            unsigned long long prod = (unsigned long long)big_digits[i] * 2 + carry;
            big_digits[i] = (unsigned int)(prod % BASE);
            carry = prod / BASE;
        }
        if (carry)
            big_digits[big_len++] = (unsigned int)carry;
    }

    /* Subtract 1 to get 2^4253 - 1. */
    size_t i = 0;
    unsigned int borrow = 1;
    while (borrow && i < big_len) {
        if (big_digits[i] >= borrow) {
            big_digits[i] -= borrow;
            borrow = 0;
        } else {
            big_digits[i] = BASE - (borrow - 1);
            borrow = 1;
        }
        ++i;
    }
    /* Remove leading zero chunks, if any. */
    while (big_len > 1 && big_digits[big_len - 1] == 0)
        --big_len;

    /* Convert to a plain decimal string. */
    decimal_len = big_len * 9; /* maximum possible length */
    /* Allocate one more byte for a null terminator */
    decimal_str = (char*)malloc(decimal_len + 10);
    size_t pos = 0;

    /* Print most significant chunk without leading zeros. */
    pos += sprintf(decimal_str + pos, "%u", big_digits[big_len - 1]);

    /* Print remaining chunks with leading zeros (9 digits each). */
    for (int j = (int)big_len - 2; j >= 0; --j) {
        pos += sprintf(decimal_str + pos, "%09u", big_digits[j]);
    }
    decimal_str[pos] = '\0';
    decimal_len = pos;
}

/* Main sequence entry point. */
seq_t a(idx_t n)
{
    /* Offsets: indices <1281 are undefined, return 0. */
    if (n < 1281)
        return 0;

    init_big_number();

    size_t digit_pos = (size_t)(n - 1280) - 1; /* zero‑based index into decimal_str */
    if (digit_pos >= decimal_len)
        return 0; /* beyond the last digit */

    return (seq_t)(decimal_str[digit_pos] - '0');
}
