/*
 * A292962  Rectangular array by antidiagonals: T(n,m) = rank of n*(r-1+m)
 *          when all the numbers k*(r+h), where r = log(2), k>=1, h>=0, are
 *          jointly ranked.
 * https://oeis.org/A292962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the rank, after reading the array by antidiagonals, of
 *            the number n*(log 2 – 1 + m) among all numbers k*(log 2 + h)
 *            with k ≥ 1 and h ≥ 0. The rank is computed directly from the
 *            definition using only integer arithmetic
 * Range:     u8 n <= 97, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t Q = ((uint64_t)1) << 60;
/* log(2) with very high precision, used only to build the integer
   approximations below */
static const long double LOG2L =
    0.69314718055994530941723212145817656807550013436025L;
/* scaled value of log(2) (floor of log(2)*Q) */
static const uint64_t LOG2_Q = (uint64_t)(LOG2L * (long double)Q);
/* (log(2) – 1)*Q, represented exactly as a signed integer */
static const int64_t OFFSET = (int64_t)LOG2_Q - (int64_t)Q;

/* Compute the rank of n*(log2 – 1 + m) among all k*(log2 + h).  The
   rank is the number of pairs (k,h) with k*(log2 + h) < n*(log2 – 1 + m)
   plus 1. */
static seq_t rank_nm(idx_t n, idx_t m)
{
    /* Y_scaled = n * ((log2 – 1) + m) * Q   (exact integer) */
    __int128 inner   = (__int128)OFFSET + (__int128)m * Q;   /* (log2‑1+m)*Q */
    __int128 Y_scaled = (__int128)n * inner;

    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        __int128 numer = Y_scaled - (__int128)k * LOG2_Q;   /* D_scaled   */
        if (numer <= 0) break;                            /* no more pairs */
        __int128 denom = (__int128)k * Q;
        /* ceil(numer / denom) = (numer + denom – 1) / denom */
        seq_t val = (seq_t)((numer + denom - 1) / denom);
        sum += val;
    }
    return sum + 1;   /* rank = number of smaller pairs + 1 */
}

/* a(n) – sequence entry for the given index n (1‑based).  The index
   is converted to a pair (row,column) in the array read by antidiagonals
   and then the corresponding rank is computed. */
seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 1;
    while (((__int128)d * (d + 1)) / 2 < n) ++d;

    /* Zero‑based offset within this diagonal */
    idx_t offset = n - (((__int128)d * (d - 1)) / 2) - 1;

    /* Convert to (row,column) = (n,m) in T(n,m) */
    idx_t row = offset + 1;
    idx_t col = d - offset;

    return rank_nm(row, col);
}
