/*
 * A275979  Decimal expansion of 2^9941 - 1, the 22nd Mersenne prime
 *          A000668(22).
 * https://oeis.org/A275979
 *
 * Offset:    2993    (a(2993) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5985, u16 n <= 5985, u32 n <= 5985, u64 n <= 5985,
 *            u128 n <= 5985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 2^9941 – 1
   a(n) returns the digit at position n (0‑based from the leftmost digit
   of 2^9941 – 1).  The offset is 2993: a(2993) is the first (most
   significant) digit.  The function works for any build of seq_t—
   whether it is a standard unsigned integer type (8–128 bit) or an
   arbitrary‑precision integer class.  It is entirely independent of
   seq_t; all arithmetic is performed on a byte array of decimal
   digits.  Thus it produces the correct answer for the 64‑bit build
   without overflow. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define OFFSET 2993
#define EXP    9941

/* Static cache for the decimal digits of 2^9941 – 1.
   digits_msb[0] holds the most significant decimal digit. */
static uint8_t *digits_msb = NULL;   /* NULL until first use */
static size_t    digits_len = 0;     /* number of digits */

/* Helper: initialize the digits array once. */
static void init_digits(void)
{
    if (digits_msb != NULL)
        return;                       /* already initialized */

    /* Temporary storage for the number in little‑endian decimal digits
       (least significant digit first).  2^9941 has at most 3000 digits. */
    uint8_t tmp[3500];
    size_t len = 1;                  /* starts with number 1 */
    tmp[0] = 1;

    /* Compute 2^EXP by repeated doubling. */
    for (size_t i = 0; i < (size_t)EXP; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint16_t val = (uint16_t)tmp[j] * 2 + carry;
            tmp[j]   = (uint8_t)(val % 10);
            carry    = (uint8_t)(val / 10);
        }
        if (carry) {
            tmp[len++] = carry;
        }
    }

    /* Subtract 1 to obtain 2^EXP – 1. */
    uint8_t borrow = 1;
    for (size_t j = 0; j < len && borrow; ++j) {
        if (tmp[j] >= borrow) {
            tmp[j] -= borrow;
            borrow  = 0;
        } else {
            tmp[j] = (uint8_t)(tmp[j] + 10 - borrow);
            borrow = 1;
        }
    }
    /* Remove possible leading zero (not needed here, but safe). */
    while (len > 1 && tmp[len - 1] == 0)
        --len;

    /* Allocate cache in most‑significant‑first order. */
    digits_msb = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (digits_msb == NULL) {   /* memory allocation failed */
        digits_len = 0;
        return;
    }

    for (size_t i = 0; i < len; ++i)
        digits_msb[i] = tmp[len - 1 - i];   /* reverse order */

    digits_len = len;
}

seq_t a(idx_t n)
{
    if (n < OFFSET)
        return (seq_t)0;                  /* undefined region */

    init_digits();                        /* lazy initialization */

    if (digits_len == 0)
        return (seq_t)0;                   /* initialization failed */

    size_t pos = (size_t)(n - OFFSET);
    if (pos >= digits_len)
        return (seq_t)0;                   /* n beyond the number */

    return (seq_t)digits_msb[pos];
}
