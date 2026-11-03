/*
 * A368928  Triangle read by rows where T(n,k) is the number of labeled
 *          loop-graphs with n vertices and n edges, k of which are loops.
 * https://oeis.org/A368928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8*n+1)−1)/2⌋, k = n − r*(r+1)/2,
 *            T(r,k) = C(r,k) * C(C(r,2), r−k) (C = binomial)
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 350,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 1293 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute binomial(N, K) for a generic (possibly huge) N using aseq var
   type seq_t and an idx_t K.  N is given as seq_t, K as idx_t (≤N). */
static seq_t binom_seq_big(seq_t N, idx_t K)
{
    if (K == 0)
        return (seq_t)1;
    if (K > N)
        return (seq_t)0;   /* not needed in our use, but safe */

    idx_t k = K;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t term = N - (seq_t)(k - i);   /* N - k + i */
        result = result * term;
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8*n+1)-1)/2) */
    seq_t t = (seq_t)8 * (seq_t)n + (seq_t)1;
    seq_t s = floor_sqrt(t);          /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((s - (seq_t)1) / (seq_t)2);

    /* k = n - r*(r+1)/2 */
    seq_t base = (seq_t)r * ((seq_t)r + (seq_t)1) / (seq_t)2;
    seq_t kseq = (seq_t)n - base;
    idx_t k = (idx_t)kseq;

    /* left part: C(r,k) */
    seq_t left = binom_seq_big((seq_t)r, k);

    /* middle part: C(r,2) */
    seq_t choose2 = (seq_t)0;
    if (r >= 2)
        choose2 = (seq_t)r * ((seq_t)r - (seq_t)1) / (seq_t)2;

    /* right part: C(C(r,2), r-k) */
    seq_t right = binom_seq_big(choose2, (idx_t)(r - k));

    return left * right;
}
