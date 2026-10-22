/*
 * A353147  Decimal repunits written in base 8.
 * https://oeis.org/A353147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal repunit of length n written in base 8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Computes OEIS sequence A353147: decimal repunits written in base 8. */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Step 1: compute the decimal repunit R_n = (10^n - 1)/9
     * by iteratively multiplying by 10 and adding 1. */
    seq_t rep = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        rep = rep * (seq_t)10 + (seq_t)1;
    }

    /* Step 2: convert rep to base 8 and store the octal digits in
     * a buffer.  Digits are stored least‑significant first. */
    /* Estimate maximum number of octal digits: n*2 is safely large */
    size_t buf_size = (size_t)n * 2 + 16;
    unsigned char *digits = (unsigned char*)malloc(buf_size);
    if (!digits) {
        /* On allocation failure, fall back to zero (should never happen) */
        return (seq_t)0;
    }

    size_t len = 0;
    seq_t tmp = rep;
    while (tmp != (seq_t)0) {
        unsigned digit = (unsigned)(tmp % (seq_t)8);
        digits[len++] = (unsigned char)digit;
        tmp /= (seq_t)8;
        /* Grow buffer if necessary (rare for the sizes considered) */
        if (len == buf_size) {
            buf_size *= 2;
            unsigned char *newdig = (unsigned char*)realloc(digits, buf_size);
            if (!newdig) {
                free(digits);
                return (seq_t)0;
            }
            digits = newdig;
        }
    }

    /* Step 3: build the resulting decimal number from the octal digits.
     * Process digits from most significant to least significant. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[len - 1 - i];
    }

    free(digits);
    return result;
}
