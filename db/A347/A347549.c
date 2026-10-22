/*
 * A347549  Number of partitions of n into 4 or more distinct parts.
 * https://oeis.org/A347549
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at least 4 distinct parts
 *            (OEIS A347549). Computation is via dynamic programming for
 *            partitions into distinct parts; the result for n ≥ 10 is then
 *            obtained by removing partitions with 1, 2 or 3 parts
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */

/* static state for memoisation */
static seq_t *part_distinct = NULL;   /* dp array holding p(k) for k <= max_n_cached */
static idx_t  max_n_cached   = 0;

/* Allocate and compute partitions into distinct parts for all k <= limit. */
static void recompute_dp(idx_t limit)
{
    /* allocate array of seq_t of length limit+1 */
    seq_t *newP = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!newP) exit(1);   /* out of memory */

    /* initialise to zero */
    memset(newP, 0, (size_t)(limit + 1) * sizeof(seq_t));

    newP[0] = 1;   /* one way to partition 0 */

    for (idx_t coin = 1; coin <= limit; ++coin) {
        for (idx_t sum = limit; sum >= coin; --sum) {
            newP[sum] = newP[sum] + newP[sum - coin];
        }
    }

    /* free previous cache if any */
    if (part_distinct) free(part_distinct);

    part_distinct = newP;
    max_n_cached = limit;
}

/* Number of partitions of n into 4 or more distinct parts. */
seq_t a(idx_t n)
{
    if (n < 10)                 /* behaviour below the offset does not matter */
        return 0;

    if (n > max_n_cached)
        recompute_dp(n);        /* ensure we have computed up to n */

    /* total partitions into distinct parts */
    seq_t total = part_distinct[(size_t)n];

    /* subtract partitions with at most 3 distinct parts */
    seq_t sub = 0;

    /* 1‑part partitions: always 1 (n itself) */
    sub += 1;

    /* 2‑part distinct partitions: floor((n-1)/2) unordered pairs */
    idx_t count2 = (n - 1) / 2;
    sub += (seq_t)count2;

    /* 3‑part distinct partitions: floor((n^2 - 6n + 12)/12) */
    idx_t temp   = n * n - 6 * n + 12;
    idx_t count3 = temp / 12;
    sub += (seq_t)count3;

    /* result cannot be negative for n >= 10 */
    if (total < sub)
        return 0;
    return total - sub;
}
