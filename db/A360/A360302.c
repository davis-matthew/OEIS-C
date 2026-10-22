/*
 * A360302  T(n,k) is the position of the set encoded in the binary expansion
 *          of k within the shortlex order for the powerset of [n]; triangle
 *          T(n,k), n>=0, 0<=k<=2^n-1, read by rows.
 * https://oeis.org/A360302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = T(r, k) where r = floor_log2(n+1), k = n - (2^r - 1);
 *            T(r,k) = Σ_{i=0}^{s-1} C(r,i) + rank_of_subset(k,r,s), s =
 *            popcount(k). The rank_of_subset counts subsets of size s that
 *            are lexicographically smaller than the set encoded by k in
 *            shortlex order
 * Range:     u8 n <= 541, u16 n <= 9725, u32 n <= 9725, u64 n <= 9725,
 *            u128 n <= 9725, big any n
 * Helpers:   binom, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/floor_log2_uint64.h"

/* Compute the rank of the subset encoded by k in shortlex order for [r] */
static seq_t compute_T(idx_t r, idx_t k)
{
    /* number of elements in the subset */
    idx_t s = __builtin_popcountll(k);

    /* sum of all subsets of smaller cardinalities */
    seq_t sum_smaller = 0;
    for (idx_t i = 0; i < s; ++i)
        sum_smaller += binom(r, i);

    /* rank among subsets of equal cardinality */
    seq_t rank = 0;
    idx_t prev = 0;           /* last chosen element (1‑based) */
    idx_t remaining = s;      /* elements still to be processed */

    for (idx_t pos = 1; pos <= r; ++pos) {
        if (k & ((idx_t)1 << (pos - 1))) {   /* element pos is in the set */
            for (idx_t x = prev + 1; x <= pos - 1; ++x) {
                rank += binom(r - x, remaining - 1);
            }
            prev = pos;
            remaining -= 1;
        }
    }

    return sum_smaller + rank;
}

seq_t a(idx_t n)
{
    /* identify the row r and column k for global index n */
    idx_t r = floor_log2_uint64(n + 1);
    idx_t prefix = ((idx_t)1 << r) - 1;
    idx_t k = n - prefix;
    return compute_T(r, k);
}
