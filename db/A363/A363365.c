/*
 * A363365  Array read by ascending antidiagonals: A(1, k) = k; for n > 1,
 *          A(n, k) = (k + 1)*A(n-1, k) + k + 1 - n, with k > 0.
 * https://oeis.org/A363365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = ((k-1)*(k+1)^(n+1)+k*n-k^2+1)/k^2. Sequence read by
 *            ascending antidiagonals, with each diagonal traversed from the
 *            bottom element upwards (i decreasing)
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A363365.  */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but safe fallback */

    /* Find the diagonal s such that (s-1)*s/2 >= n.
       Diagonal s has s-1 elements. */
    idx_t s = 2;
    while (( (__uint128_t)(s - 1) * s / 2 ) < n)
        ++s;

    /* Total terms before this diagonal: (s-2)*(s-1)/2 */
    idx_t prev = ( (__uint128_t)(s - 2) * (s - 1) / 2 );
    idx_t pos = n - prev;                  /* 1‑based position within diagonal */
    idx_t i   = s - pos;                   /* row index (n) */
    idx_t j   = s - i;                     /* column index (k) */

    /* Compute A(i,j) via the closed form */
    seq_t base  = (seq_t)(j + 1);
    seq_t power = pow_seq(base, i + 1);

    seq_t term1 = (seq_t)(j - 1) * power;
    seq_t term2 = (seq_t)j * (seq_t)i;
    seq_t term3 = -(seq_t)j * (seq_t)j;
    seq_t num   = term1 + term2 + term3 + 1;
    seq_t denom = (seq_t)j * (seq_t)j;

    return num / denom;
}
