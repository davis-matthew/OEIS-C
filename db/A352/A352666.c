/*
 * A352666  Maximum number of induced copies of the claw graph K_{1,3} in an
 *          n-node graph.
 * https://oeis.org/A352666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = max_{1≤k≤n-1} [ k*binom(n−k,3) + (n−k)*binom(k,3) ]
   optimal k is the part size of the bipartite K_{k,n−k} that maximises
   the number of induced copies of the claw K_{1,3}.
   The optimal k is one of the two integers nearest to
   n/2 − sqrt(3*n/4 − 1). */

#include <stddef.h>
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

static seq_t binom3(idx_t n) {
    if (n < 3) return 0;
    return (seq_t)n * (n - 1) * (n - 2) / 6;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;

    seq_t best = 0;

    /* approximate optimal part size k0 */
    seq_t three_n_minus_four = (seq_t)(3 * n - 4);
    seq_t sqrt_floor = floor_sqrt(three_n_minus_four);
    seq_t n_half = (seq_t)n / 2;
    seq_t sqrt_half = sqrt_floor / 2;
    seq_t k0 = n_half - sqrt_half;   /* may be 0 for very small n */

    /* examine a few candidates around k0 */
    idx_t start = (idx_t)(k0 > 2 ? k0 - 2 : 1);
    idx_t end   = (idx_t)( ((idx_t)k0 + 2) < (n - 1) ? k0 + 2 : n - 1);

    for (idx_t k = start; k <= end; ++k) {
        seq_t term1 = (seq_t)k * binom3(n - k);
        seq_t term2 = (seq_t)(n - k) * binom3(k);
        seq_t val = term1 + term2;
        if (val > best) best = val;
    }
    return best;
}
