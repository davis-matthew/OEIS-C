/*
 * A360971  Number of multisets of size n with elements from [n] whose element
 *          sum is larger than the product of all elements.
 * https://oeis.org/A360971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multisets of size n with elements in [n] whose
 *            element sum is larger than the product of all elements
 * Range:     u8 n <= 6, u16 n <= 91, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper that enumerates multiplicities of values >= start. */
static void dfs(idx_t start, idx_t rem, seq_t prod, seq_t sum_non,
                idx_t t_total, seq_t *total, idx_t n, uint64_t limit64)
{
    if (rem == 0) {
        seq_t total_sum = (seq_t)n - (seq_t)t_total + sum_non;
        if (total_sum > prod) (*total)++;
        return;
    }
    if (start > n) return;

    for (idx_t val = start; val <= n; ++val) {
        uint64_t val64 = (uint64_t)val;
        /* If product will already exceed the global limit for a single copy of 'val',
           then larger values will also. */
        if (prod > (seq_t)(limit64 / val64)) break;

        seq_t prod_local = prod;
        seq_t sum_local = sum_non;

        for (idx_t cnt = 1; cnt <= rem; ++cnt) {
            prod_local = prod_local * val;
            if ((uint64_t)prod_local > limit64) break;
            sum_local += val;

            if (rem - cnt == 0) {
                seq_t total_sum = (seq_t)n - (seq_t)t_total + sum_local;
                if (total_sum > prod_local) (*total)++;
            } else {
                dfs(val + 1, rem - cnt, prod_local, sum_local,
                    t_total, total, n, limit64);
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the value is 0. */
    if (n <= 1) return 0;

    /* Compute tmax: maximum number of elements > 1 that can appear. */
    idx_t tmax = 0;
    seq_t pow2 = 1;
    while (1) {
        uint64_t rhs = (uint64_t)n + (uint64_t)tmax * ((uint64_t)n - 1);
        if ((uint64_t)pow2 > rhs) break;
        tmax++;
        pow2 <<= 1;
    }
    if (tmax > n) tmax = n;               /* cannot have more than n such elements */

    /* Global pruning limit.  Product never exceeds n*(tmax+1). */
    uint64_t limit64 = (uint64_t)n * ((uint64_t)tmax + 1);

    seq_t total = 0;

    /* The multiset consisting entirely of ones is valid for n > 1. */
    total += 1;     /* [1,1,…,1] */

    /* Enumerate all multisets with t non‑ones for 1 ≤ t ≤ tmax. */
    for (idx_t t = 1; t <= tmax; ++t) {
        dfs(2, t, (seq_t)1, (seq_t)0, t, &total, n, limit64);
    }

    return total;
}
