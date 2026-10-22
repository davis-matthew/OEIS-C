/*
 * A319526  Square array read by antidiagonals upwards: T(n,k) = sigma(n*k), n
 *          >= 1, k >= 1.
 * https://oeis.org/A319526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sigma( i*j ) where (i,j) are the antidiagonal coordinates
 *            of index n. (i + j = d+1, d minimal with d*(d+1)/2 >= n)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma(idx_t n)
{
    seq_t result = 1;
    idx_t x = n;
    for (idx_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            seq_t sum = 1;
            seq_t pow = 1;
            while (x % p == 0) {
                x /= p;
                pow = pow * (seq_t)p;
                sum += pow;
            }
            result *= sum;
        }
    }
    if (x > 1) {
        result *= ((seq_t)1 + (seq_t)x);
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while ( ((__int128)d * (d + 1)) / 2 < n ) {
        ++d;
    }

    /* Position offset within the diagonal (0‑based) */
    idx_t prev = ((__int128)(d - 1) * d) / 2;
    idx_t offset = n - prev - 1;          /* 0‑based index in the diagonal */

    /* Antidiagonal coordinates: (i,j) with i+j = d+1 */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Product i*j is small enough for idx_t */
    idx_t prod = i * j;

    return sigma(prod);
}
