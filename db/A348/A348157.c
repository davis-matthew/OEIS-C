/*
 * A348157  Table read by antidiagonals: T(n,k) = number of factorizations of
 *          (n,k) into one or two pairs (i,j) with i > 0, j > 0 (and if i=1
 *          then j=1).
 * https://oeis.org/A348157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n = 1 or the antidiagonal coordinates (i,j) satisfy
 *            * i = 1 and j = 1. For all other n, let (i,j) be the coordinates
 *            of the * n‑th element in the table read by antidiagonals, and
 *            let τ(x) be the * number of divisors of x. Then * a(n) = ceil(
 *            (τ(i) – 2)·τ(j) / 2 ) + 1 (OEIS formula for...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a __int128 value (floor). */
static inline uint64_t isqrt_u128(__int128 x)
{
    uint64_t r = 0;
    for (int shift = 63; shift >= 0; --shift) {
        uint64_t bit = 1ULL << shift;
        __int128 val = r + bit;
        if (val != 0 && val <= x / val) {
            r += bit;
        }
    }
    return r;
}

/* Return the number of divisors τ(n) for n ≥ 1.  n fits in uint64_t,
 * and τ(n) comfortably fits in seq_t. */
static inline seq_t tau_uint64(uint64_t n)
{
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (m > 1) {
        result *= 2;
    }
    return result;
}

/* Translate 1‑based index n into antidiagonal coordinates (i,j).
 * The table is read by increasing sums s = i + j, and for each sum
 * the pairs are enumerated with i ascending from 1 to s‑1. */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t k = n - 1;                                 /* zero‑based offset */
    __int128 discr = (__int128)8 * (__int128)k + 1;   /* discriminant 8k+1 */
    uint64_t sq = isqrt_u128(discr);                  /* floor(sqrt) */
    uint64_t t = (sq - 1) / 2;                        /* diagonal index (0‑based) */
    idx_t cum_before = t * (t + 1) / 2;               /* elements before this diag */
    idx_t offset = k - cum_before;                   /* position inside diagonal */
    *i = offset + 1;
    *j = (t + 1) - offset;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for out‑of‑range call */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    if (i == 1) {
        return (j == 1) ? (seq_t)1 : (seq_t)0;
    }

    seq_t tau_i = tau_uint64(i);
    seq_t tau_j = tau_uint64(j);

    seq_t temp = (tau_i - (seq_t)2) * tau_j;          /* non‑negative for i > 1 */
    seq_t val  = ((temp + (seq_t)1) / (seq_t)2) + (seq_t)1;
    return val;
}
