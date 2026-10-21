/*
 * A279426  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 6.
 * https://oeis.org/A279426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2495, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer k such that k² has an odd number of decimal
   digits and the middle digit of that square is 6 (OEIS A279426, offset 1). */
#include <stdint.h>

static int digit_len_idx(idx_t x)
{
    int len = 0;
    do { ++len; x /= 10; } while (x);
    return len;
}

static idx_t pow10_idx(unsigned exp)
{
    idx_t p = 1;
    while (exp--) p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* defined only for n ≥ 1 */
        return 0;

    idx_t k = 1;                /* candidate number */
    idx_t found = 0;            /* how many valid k's seen */

    while (1) {
        idx_t sq = k * k;                /* k² in idx_t, always fits */
        int len = digit_len_idx(sq);     /* number of decimal digits */

        if (len & 1) {                   /* odd number of digits? */
            idx_t half = len >> 1;       /* half the digit count      */
            idx_t pow10 = pow10_idx((unsigned)half); /* 10^half       */
            idx_t mid = (sq / pow10) % 10;            /* middle digit  */
            if (mid == 6) {                     /* middle is 6?      */
                ++found;
                if (found == n)
                    return (seq_t)k;           /* found n‑th term   */
            }
        }
        ++k;
    }
}
