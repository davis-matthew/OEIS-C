/*
 * A290327  Total number of parts in all partitions of n into distinct Lucas
 *          numbers (beginning with 1) (A000204).
 * https://oeis.org/A290327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Total number of parts in all partitions of n into
 *            distinct Lucas numbers (beginning with 1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over the set of admissible Lucas numbers.
   For each Lucas number v we update the subset‑sum tables in descending order
   so that each new subset that includes v contributes its part count
   exactly once.  Complexity O(K·n), with K ≈ 19 for n ≤ 10000. */

#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Generate Lucas numbers ≥ 1 and ≤ n.
       Lucas numbers (starting with 1): 1,3,4,7,11,18,29,47,76,123,199,322,521,843,1364,2207,3571,
       5778,9349 (≤10000).  We use a fixed-size array. */
    uint64_t lucas[32];
    size_t lucas_len = 0;
    if (n >= 1) {
        lucas[lucas_len++] = 1ULL;
    }
    if (n >= 3) {
        lucas[lucas_len++] = 3ULL;
    }
    uint64_t a_l = 1ULL, b_l = 3ULL;
    while (true) {
        uint64_t c_l = a_l + b_l;
        if (c_l > n) break;
        a_l = b_l;
        b_l = c_l;
        lucas[lucas_len++] = c_l;
    }

    /* Allocate DP tables for count of subsets and total parts. */
    seq_t *count = alloc_seq_array(n + 1);
    seq_t *parts = alloc_seq_array(n + 1);
    count[0] = (seq_t)1;   /* empty subset */
    /* All other entries are already zero-initialised by alloc_seq_array. */

    /* DP over each Lucas number. */
    for (size_t i = 0; i < lucas_len; ++i) {
        idx_t v = (idx_t)lucas[i];
        for (idx_t s = n; s >= v; --s) {
            seq_t c_old = count[s - v];
            if (c_old == 0) continue;   /* no subsets that can be extended with v */
            seq_t p_old = parts[s - v];
            count[s] += c_old;                       /* new subsets */
            parts[s] += p_old + c_old;               /* each such subset gains one more part */
        }
    }

    seq_t result = parts[n];

    /* Free allocated memory. */
    free_seq_array(count);
    free_seq_array(parts);

    return result;
}
