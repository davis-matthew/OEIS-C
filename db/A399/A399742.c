/*
 * A399742  Table of B(n, m) read by antidiagonals: B(n, m) = Sum_{k=0..m-1}
 *          binomial(k+n, n) * 2^k and B(n, 0) = 0, for n >= 0, m >= 0.
 * https://oeis.org/A399742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = B(i,j) where (i,j) = antidiagonal_coords(n) and B(n,m) =
 *            Sum_{k=0}^{m-1} binomial(k+n, n)*2^k. Implemented via the
 *            recurrence B(n,m)=B(n,m-1)+binomial(m+n-1,n)*2^(m-1)
 * Range:     u8 n <= 28, u16 n <= 80, u32 n <= 280, u64 n <= 958,
 *            u128 n <= 3593, big any n
 * Helpers:   antidiagonal_coords, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    seq_t sum = 0;
    seq_t pow2 = 1;            /* 2^(m-1)  for current m */
    for (idx_t m = 1; m <= j; ++m) {
        seq_t coeff = binom(m + i - 1, i);   /* binomial(m+i-1, i) */
        sum += coeff * pow2;                  /* accumulate term */
        pow2 <<= 1;                           /* next power of two */
    }
    return sum;
}
