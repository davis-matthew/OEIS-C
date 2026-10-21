/*
 * A275329  a(n) = (2+[n/2])*n!/((1+[n/2])*[n/2]!^2).
 * https://oeis.org/A275329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + ⌊n/2⌋) * n! / ((1 + ⌊n/2⌋) * ⌊n/2⌋!²)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: for odd n = 2k+1: a(n) = (k+2) * binomial(2k+1, k+1) */
/*              for even n = 2k:   a(n) = (k+2)/(k+1) * binomial(2k, k) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k;
    seq_t res;

    if ((n & 1) == 0) {                /* even n = 2k */
        k = n / 2;
        res = binomial(2 * k, k);          /* binomial(2k, k) */
        res = res * ((seq_t)k + 2);         /* multiply by (k+2) */
        res = res / ((seq_t)k + 1);         /* divide by (k+1) */
    } else {                           /* odd n = 2k+1 */
        k = (n - 1) / 2;
        res = binomial(2 * k + 1, k + 1);    /* binomial(2k+1, k+1) */
        res = res * ((seq_t)k + 2);          /* multiply by (k+2) */
    }

    return res;
}
