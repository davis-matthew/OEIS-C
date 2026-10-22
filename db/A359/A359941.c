/*
 * A359941  Irregular triangle read row by row. The k-th row are integers from
 *          0 to 2^k-1 in base 2 ordered in graded reverse lexicographical
 *          order.
 * https://oeis.org/A359941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 519, u16 n <= 9954, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   binom_int, unrank_row_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A359941
    An irregular triangle read row by row.
    For row k (k ≥ 0), list the integers {0, … , 2ⁿ-1} sorted by
      • Hamming weight (ascending)
      • For equal weight, by increasing lexicographic order of the
        indices of set bits (equivalently, by the order of combinations
        of indices in standard lexicographic order).

    The function below computes a(n) directly using combinatorial
    enumeration and unranking, without memoisation.
    Complexity is O(k²) with k = ⌊log₂(n+1)⌋, which is negligible for
    the required limits (k ≤ 13 for n ≤ 10000).    */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_int.h"
#include "../utils/unrank_row_value.h"

/* convenient integer types supplied by the build
 *   idx_t   – unsigned 64‑bit integer (uint64_t)
 *   seq_t   – unsigned integer type of arbitrary width (uint8_t … unsigned __int128)
 */

/* helper: floor of binary logarithm */
static inline unsigned floor_log2_uint64(idx_t v)
{
    /* v ≥ 1 for all uses in this program */
    return 63u - __builtin_clzll((unsigned long long)v);
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* determine row k */
    unsigned k = floor_log2_uint64(n + 1);
    idx_t start = ((idx_t)1 << k) - 1;          /* first index of row k */
    idx_t offset = n - start;                   /* offset within row */

    idx_t remaining = offset;
    for (idx_t w = 0; w <= k; ++w)
    {
        idx_t cnt = binom_int(k, w);
        if (remaining < cnt)
        {
            /* we found the weight w and the rank within that weight */
            return (w == 0) ? (seq_t)0 : unrank_row_value(k, w, remaining);
        }
        remaining -= cnt;
    }
    /* unreachable for valid input */
    return (seq_t)0;
}
