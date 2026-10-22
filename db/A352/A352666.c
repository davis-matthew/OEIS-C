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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = max_{1≤k≤n-1} [ k*binom(n−k,3) + (n−k)*binom(k,3) ]
   optimal k is the part size of the bipartite K_{k,n−k} that maximises
   the number of induced copies of the claw K_{1,3}.
   The optimal k is one of the two integers nearest to
   n/2 − sqrt(3*n/4 − 1). */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

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
