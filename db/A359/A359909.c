/*
 * A359909  Number of integer factorizations of n into factors > 1 with the
 *          same mean as median.
 * https://oeis.org/A359909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8787, u16 n <= 8787, u32 n <= 8787, u64 n <= 8787,
 *            u128 n <= 8787, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count factorizations of n into factors > 1 whose arithmetic mean equals
 * the median of the multiset of factors.
 * Each factorization is considered as a multiset (order irrelevant).
 * The function uses depth‑first search generating factors in non‑decreasing
 * order, guaranteeing each multiset is counted exactly once. */
#include <stdint.h>
#include <stddef.h>

static void dfs(idx_t rem, idx_t min_f, idx_t depth,
                idx_t *factors, idx_t sum, idx_t *cnt)
{
    /* Count the factorization that uses all chosen factors plus the
     * remaining factor `rem` (if rem is >= min_f). */
    if (rem >= min_f) {
        idx_t total_sum = sum + rem;
        idx_t length = depth + 1;

        /* Find the median(s) of the multiset.  The last element
         * (rem) occupies position `depth` in the ordered list. */
        if (length & 1U) { /* odd length */
            idx_t mid = length >> 1;           /* middle index */
            idx_t median = (mid == depth) ? rem : factors[mid];
            if (total_sum == length * median)
                ++(*cnt);
        } else { /* even length */
            idx_t mid = length >> 1;
            idx_t left = (mid - 1 < depth) ? factors[mid - 1] : rem;
            idx_t right = (mid < depth)    ? factors[mid]     : rem;
            if (2 * total_sum == length * (left + right))
                ++(*cnt);
        }
    }

    /* Explore further factors bigger or equal to min_f. */
    for (idx_t f = min_f; f * f <= rem; ++f) {
        if (rem % f == 0) {
            factors[depth] = f;
            dfs(rem / f, f, depth + 1, factors, sum + f, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) /* a(1) = 0 by definition */
        return 0;

    idx_t factors[64], count = 0; /* 64 is ample for n <= 10 000 */
    dfs(n, 2, 0, factors, 0, &count);
    return (seq_t)count;
}
