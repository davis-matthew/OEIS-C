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
 * Validated: standard, pass, 82 terms through n = 9565 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

seq_t a(idx_t n) {
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    seq_t sum = 0;
    seq_t pow2 = 1;            /* 2^(m-1)  for current m */
    for (idx_t m = 1; m <= j; ++m) {
        seq_t coeff = binomial(m + i - 1, i);   /* binomial(m+i-1, i) */
        sum += coeff * pow2;                  /* accumulate term */
        pow2 <<= 1;                           /* next power of two */
    }
    return sum;
}
