/*
 * A392945  Irregular triangular array T(n,k) read by rows. Row n gives the
 *          number of permutations of {1,2,...,n} whose descent set is
 *          contained in S for each subset S of {1,2,...,n-1} where the
 *          subsets are ordered first by size and then lexicographically (Cf.
 *          A082185).
 * https://oeis.org/A392945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = value of the triangular array T(n,S) read by rows where S
 *            is a subset of [n-1] ordered first by size then
 *            lexicographically
 * Range:     u8 n <= 57, u16 n <= 475, u32 n <= 8190, u64 n <= 8839,
 *            u128 n <= 8839, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

/* Compute floor(log2(v)) for v>0.  v is idx_t (uint64_t). */
static inline idx_t floor_log2_idx(idx_t v) {
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* a(0) is the first element of row 0, which is 1. */
    if (n == 0) return 1;

    /* Determine the row r (0‑based). */
    idx_t r = floor_log2_idx(n) + 1;                 /* r >= 1 for n>=1 */
    idx_t start = (idx_t)1 << (r - 1);               /* first index of row r */
    idx_t pos   = n - start;                         /* position within the row */

    /* Find the size k of the subset corresponding to position pos. */
    idx_t cumulative = 0;
    idx_t k = 0;
    for (k = 0; k <= r - 1; ++k) {
        seq_t comb_val = binomial(r - 1, k);
        idx_t comb = (idx_t)comb_val;               /* fits into idx_t for r<=64 */
        if (cumulative + comb > pos) break;
        cumulative += comb;
    }
    idx_t rank_in_k = pos - cumulative;             /* rank inside size‑k subsets */

    /* Build the subset of size k that has lexicographic rank rank_in_k. */
    /* For r <= 64 this array is more than large enough. */
    idx_t subset[64];
    idx_t remaining = rank_in_k;
    idx_t next_start = 1;                           /* next candidate value */

    for (idx_t j = 0; j < k; ++j) {
        for (idx_t val = next_start; val <= (r - 1) - (k - j) + 1; ++val) {
            seq_t comb_val = binomial(r - 1 - val, k - j - 1);
            idx_t comb = (idx_t)comb_val;
            if (remaining >= comb) {
                remaining -= comb;
            } else {
                subset[j] = val;
                next_start = val + 1;
                break;
            }
        }
    }

    /* Compute the multinomial coefficient: */
    /* T(r, subset) = r! / (s1! (s2-s1)! … (r-s_k)!) */
    seq_t result = 1;
    idx_t prev = 0;
    idx_t rem  = r;
    for (idx_t j = 0; j < k; ++j) {
        idx_t seg = subset[j] - prev;                /* length of this part */
        result = result * binomial(rem, seg);
        rem -= seg;
        prev = subset[j];
    }
    /* The last segment contributes *C(rem, rem) = 1, so we can skip it */

    return result;
}
