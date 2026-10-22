/*
 * A359911  Number of integer factorizations of n into factors > 1 without the
 *          same mean as median.
 * https://oeis.org/A359911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001055(n) - A359909(n)
 * Range:     u8 n <= 2879, u16 n <= 9329, u32 n <= 9329, u64 n <= 9329,
 *            u128 n <= 9329, big any n
 * Helpers:   bad_partition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count all multiplicative factorizations of n into factors >1,
   then subtract those whose mean equals the median.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/bad_partition.h"

/* Recursive enumeration of all multiplicative partitions of `n`.
   `min_factor` enforces non‑decreasing order of factors to avoid duplicates.
   `factors` holds the currently built factor list; `len` is its length.
   `sum` accumulates the sum of the current factors.
   `total` and `bad` count, respectively, all partitions and those with mean=median. */
static void dfs(idx_t n, idx_t min_factor,
                seq_t *factors, size_t len, seq_t sum,
                seq_t *total, seq_t *bad)
{
    for (idx_t f = min_factor; f * f <= n; ++f) {
        if (n % f == 0) {
            factors[len] = (seq_t)f;
            dfs(n / f, f, factors, len + 1, sum + (seq_t)f, total, bad);
        }
    }
    /* Final factorization by taking the remaining `n` itself. */
    if (n >= min_factor) {
        factors[len] = (seq_t)n;
        seq_t total_sum = sum + (seq_t)n;
        size_t L = len + 1;
        (*total) += 1;
        if (bad_partition(factors, L, total_sum))
            (*bad) += 1;
    }
}

/* External linkage function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 2)            /* 1 has no factorizations into factors >1 */
        return 0;
    seq_t total = 0, bad = 0;
    static seq_t factors[64];   /* depth is never > 60 for n ≤ 10^4 */
    dfs(n, 2, factors, 0, (seq_t)0, &total, &bad);
    return total - bad;
}
