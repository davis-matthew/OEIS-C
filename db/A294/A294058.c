/*
 * A294058  Number of set partitions of [n] such that the maximal absolute
 *          difference between consecutive elements within a block equals
 *          nine.
 * https://oeis.org/A294058
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A287281(n) - A287280(n)
 * Range:     u8 none, u16 n <= 11, u32 n <= 16, u64 n <= 28, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 985 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A287281(n) = Number of set partitions of [n] whose maximal
 * consecutive difference ≤ 9.
 * A287280(n) = same with bound ≤ 8. */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Count partitions of [n] with maximal consecutive difference ≤ k. */
static seq_t partitions_leq_k(idx_t n, int k)
{
    /* state mask has k+1 bits (distance 0..k), size = 2^(k+1). */
    size_t size = (size_t)1 << (k + 1);

    seq_t *dp = mem_new(seq_t, size);
    seq_t *next = mem_new(seq_t, size);
    if (!dp || !next) {
        /* Allocation failure – return 0. */
        mem_free(dp); mem_free(next);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1; /* empty partition */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear next array */
        mem_set(next, 0, size * sizeof(seq_t));

        for (size_t mask = 0; mask < size; ++mask) {
            seq_t v = dp[mask];
            if (!v) continue;

            /* shift existing blocks: distances increase by 1 */
            size_t shifted = (mask << 1) & (size - 1);

            /* Option 1: start a new block containing 'pos' */
            next[shifted | 1] += v;

            /* Option 2: extend an existing block */
            size_t bits = shifted;
            while (bits) {
                size_t bitpos = __builtin_ctzll(bits);
                bits &= bits - 1;
                size_t newmask = (shifted & ~((size_t)1 << bitpos)) | 1;
                next[newmask] += v;
            }
        }

        /* swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* sum all final states; all remaining open blocks are valid */
    seq_t res = (seq_t)0;
    for (size_t mask = 0; mask < size; ++mask)
        res += dp[mask];

    mem_free(dp);
    mem_free(next);
    return res;
}

seq_t a(idx_t n)
{
    seq_t a9 = partitions_leq_k(n, 9);
    seq_t a8 = partitions_leq_k(n, 8);
    return a9 - a8;
}
