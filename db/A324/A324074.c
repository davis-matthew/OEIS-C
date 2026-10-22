/*
 * A324074  Total number of distorted ancestor-successor pairs in all
 *          defective (binary) heaps on n elements.
 * https://oeis.org/A324074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (sum‑of‑depths in a complete binary tree of size n)
 *            / 2 The total number of ancestor pairs equals the sum of the
 *            depths of all nodes; in a heap the positions are indexed 0…n−1,
 *            so the depth of node i (i ≥ 1) is floor(log₂(i+1)). For each
 *            such pair the probability that the parent < chi...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   factorial, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* total number of ancestor–successor pairs in the complete binary tree
       of size n; node 0 is the root and has depth 0, so we start from i=1 */
    uint64_t total_pairs = 0;
    for (idx_t i = 1; i < n; ++i) {
        total_pairs += floor_log2_uint64(i + 1);
    }

    /* factorial(n) is provided in the utils library. */
    seq_t fact = factorial(n);

    /* result = fact * total_pairs / 2 */
    return (fact * (seq_t)total_pairs) / (seq_t)2;
}
