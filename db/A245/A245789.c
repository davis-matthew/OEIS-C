/*
 * A245789  Rectangular array A read by upward antidiagonals: A(k,n) =
 *          (2^k-1)^n, n,k >= 1.
 * https://oeis.org/A245789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^m - 1)^(s - m) where s is the smallest integer with
 *            (s-1)*s/2 >= n and m = n - (s-2)*(s-1)/2 (i.e., the position of
 *            n on its antidiagonal). This corresponds to reading the array
 *            A(k,n) = (2^n-1)^k by upward antidiagonals (k decreasing, n
 *            increasing)
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/pow2.h"   /* seq_t pow2(idx_t); */
#include "../utils/big_pow.h" /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* Find the antidiagonal sum s such that (s-1)*s/2 >= n. */
    idx_t s = 2;
    /* Use 128-bit arithmetic to avoid overflow on intermediate products. */
    __int128 term = (__int128)(s - 1) * s / 2;
    while ((idx_t)term < n) {
        ++s;
        term = (__int128)(s - 1) * s / 2;
    }

    /* Compute the index m within this antidiagonal:
       n_entry = n - ((s-2)*(s-1)/2)   (1 <= m <= s-1) */
    idx_t prev = ((s - 2) * (s - 1)) / 2;
    idx_t m    = n - prev;                  /* m = column index (n in A(k,n)) */
    idx_t col  = m;                          /* same as m */
    idx_t row  = s - m;                      /* k on this antidiagonal */

    /* Base is (2^col - 1). */
    seq_t base = pow2(col);
    base -= (seq_t)1;

    /* Compute base^row.  The exponent is small (row < s <= ~sqrt(2n)),
       but we use big_pow for arbitrary precision support. */
    seq_t val = big_pow(base, (seq_t)row);

    return val;
}
