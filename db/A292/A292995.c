/*
 * A292995  Sum of digits of 3^n (A004166) divided by 9.
 * https://oeis.org/A292995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_of_digits(3^n) / 9 a(0) = 0, a(1) = 0, defined for
 *            all idx_t n >= 0
 * Range:     u8 n <= 995, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define BASE 1000000000U  /* 10^9, fits in uint32_t */

/* Compute the OEIS sequence A292995. */
seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly. */
    if (n <= 1)
        return (seq_t)0;

    /* Dynamically grow an array of base-BASE blocks for the big integer 3^n. */
    size_t cap = 8;                /* initial capacity in blocks */
    size_t len = 1;                /* number of used blocks */
    uint32_t *blocks = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!blocks)
        return (seq_t)0;           /* allocation failure – return 0 safely */
    blocks[0] = 1;                 /* 3^0 = 1 */

    /* Multiply by 3, n times to obtain 3^n. */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)blocks[j] * 3U + carry;
            blocks[j] = (uint32_t)(prod % BASE);
            carry     = prod / BASE;
        }
        if (carry) {
            if (len == cap) {                     /* extend array if needed */
                cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(blocks, cap * sizeof(uint32_t));
                if (!tmp) {
                    free(blocks);
                    return (seq_t)0;               /* allocation failure */
                }
                blocks = tmp;
            }
            blocks[len++] = (uint32_t)carry;
        }
    }

    /* Compute the sum of all decimal digits of the huge integer. */
    uint64_t sum = 0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t block = blocks[j];
        while (block) {
            sum += block % 10U;
            block /= 10U;
        }
    }

    free(blocks);
    return (seq_t)(sum / 9U);
}
