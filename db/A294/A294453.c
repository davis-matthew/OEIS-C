/*
 * A294453  Array read by antidiagonals: T(0,k) = A000045(k+1) for k >= 0.
 *          T(n,0) = 1 for n >= 0; thereafter T(n,k) = T(n-1,k-1)+T(n-1,k) for
 *          n, k >= 1.
 * https://oeis.org/A294453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(n,m) = Σ_{k=0..m} binom(n+m-k,k) and (i,j)
 *            are the antidiagonal coordinates of n in the array defined by
 *            OEIS A294453
 * Range:     u8 n <= 94, u16 n <= 307, u32 n <= 1142, u64 n <= 4400,
 *            u128 n <= 9588, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the array by reading its antidiagonals directly. */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for 128‑bit values.  Used only internally. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32;            /* > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return the diagonal index d (starting from 0) that contains global
   index n.  e.g. n = 0..0 => d=0, n = 1..2 => d=1, n = 3..5 => d=2, ... */
static idx_t diagonal_of(idx_t n)
{
    idx_t d = 0;
    while (1) {
        idx_t next = d + 1;
        if ((__uint128_t)next * (next + 1) / 2 > n)
            break;
        d = next;
    }
    return d;
}

/* Compute binomial coefficient C(n,k) using a multiplicative loop.
   Intermediate calculations are performed in __uint128_t so that
   intermediate overflows are avoided.  The result is converted to
   seq_t (the build‑specific unsigned integer type). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;        /* use symmetry */
    __uint128_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return (seq_t)res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Locate the antidiagonal coordinates (i,j). */
    idx_t d      = diagonal_of(n);                    /* diagonal index   */
    idx_t prev   = d * (d + 1) / 2;                    /* elements before d */
    idx_t offset = n - prev;                          /* position in diag */
    idx_t i      = d - offset;                        /* row index        */
    idx_t j      = offset;                            /* column index     */

    /* Sum the binomial terms that define T(i,j). */
    __uint128_t sum = 0;
    for (idx_t k = 0; k <= j; ++k) {
        sum += ( __uint128_t )binom_small(i + j - k, k);
    }
    return (seq_t)sum;
}
