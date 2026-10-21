/*
 * A299768  Triangle read by rows: T(n,k) = sum of all squares of the parts k
 *          in all partitions of n, with n >= 1, 1 <= k <= n.
 * https://oeis.org/A299768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum of all squares of the part k in all partitions of n,
 *            * where the flattened index n enumerates the triangle read by
 *            rows
 * Range:     u8 n <= 57, u16 n <= 439, u32 n <= 4567, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Cached partition numbers p[0..N] (unrestricted partitions) */
static seq_t *partitions = NULL;   /* partitions[i] = p(i) */
static idx_t partition_size = 0;   /* number of entries in `partitions` (0 = uninitialized) */

/* Rebuild the partition array up to `n` inclusive */
static void rebuild_partitions(idx_t n)
{
    if (partitions != NULL)
        seq_free_array(partitions);

    partitions = seq_alloc_new_array(n + 1);
    if (!partitions)
        return;            /* allocation failed; leave partitions NULL */

    /* initialise */
    partitions[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        partitions[i] = (seq_t)0;

    /* additive DP: for each part value coin, add contributions to larger sums */
    for (idx_t coin = 1; coin <= n; ++coin) {
        for (idx_t j = coin; j <= n; ++j) {
            partitions[j] = partitions[j] + partitions[j - coin];
        }
    }

    partition_size = n + 1;
}

/* Ensure that `partitions` contains p[0]..p[n] */
static void ensure_partitions(idx_t n)
{
    if (partition_size == 0 || partition_size <= n)
        rebuild_partitions(n);
}

/* Find row r and column k for flattened index `idx` (1‑based) */
static void row_col(idx_t idx, idx_t *r, idx_t *k)
{
    idx_t rr = 0;
    while (rr * (rr + 1) / 2 < idx)
        ++rr;
    *r = rr;
    idx_t prev = (rr - 1) * rr / 2;
    *k = idx - prev;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: undefined for 0 */
        return (seq_t)0;

    idx_t r, k;
    row_col(n, &r, &k);

    /* We need partition numbers up to r */
    ensure_partitions(r);
    if (partitions == NULL)   /* allocation failed */
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t maxm = r / k;
    for (idx_t m = 1; m <= maxm; ++m) {
        idx_t rem = r - m * k;
        sum = sum + partitions[rem];
    }

    seq_t ksq = (seq_t)k * (seq_t)k;
    return ksq * sum;
}
