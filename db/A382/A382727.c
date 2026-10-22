/*
 * A382727  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 11.
 * https://oeis.org/A382727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..n} Π_j (d_j(i)+1) where d_j(i) are the base‑11
 *            digits of i. This counts non‑zero binomial coefficients in each
 *            row of Pascal's triangle modulo 11
 * Range:     u8 n <= 26, u16 n <= 611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate enough digits: 11^5 > 10000, so 6 digits suffice. */
    uint8_t digits[20] = {0};
    seq_t prod = 1;          /* product of (digit+1) for current i */
    seq_t total = 0;         /* accumulator for a(n) */

    for (idx_t i = 0; i <= n; ++i) {
        total += prod;       /* add contribution of row i */

        if (i == n) break;   /* avoid incrementing past n */

        /* Increment i in base 11, updating prod accordingly. */
        unsigned int j = 0;
        while (1) {
            uint8_t old = digits[j];
            if (++digits[j] < 11) {            /* no carry */
                prod = prod / (seq_t)(old + 1)
                       * (seq_t)(digits[j] + 1);
                break;
            } else { /* digit rolled over from 10 to 0, carry */
                digits[j] = 0;
                prod = prod / (seq_t)(old + 1); /* multiply by 1 is
                                                   no change */
                ++j; /* carry to next significant digit */
            }
        }
    }

    return total;
}
