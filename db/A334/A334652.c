/*
 * A334652  Number of integer partitions of n with at least two parts, each
 *          greater than 1 and with the same multiplicity.
 * https://oeis.org/A334652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + Σ_{d|n} A025147(d) for n > 1 A025147(d) = number of
 *            partitions of d into distinct parts ≥ 2
 * Range:     u8 n <= 33, u16 n <= 85, u32 n <= 249, u64 n <= 813,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *dp = NULL;
static idx_t   dp_cap = 0;

/* Compute partitions into distinct parts ≥ 2 for all integers up to limit */
static void init_dp_upto(idx_t limit)
{
    if ((size_t)limit <= dp_cap) return;          /* already available */

    seq_t *old = dp;
    dp_cap = limit;
    dp = seq_alloc_new_array(dp_cap + 1);
    if (!dp) abort();

    /* zero initialise */
    for (size_t i = 0; i <= dp_cap; ++i) dp[i] = 0;
    dp[0] = 1;         /* base: empty partition */

    for (size_t part = 2; part <= dp_cap; ++part) {
        for (size_t sum = dp_cap; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    if (old) seq_free_array(old);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;      /* offset 0 and 1 give 0 */

    /* pre‑compute up to 10000 once; extend if a larger n is requested */
    const idx_t PRECOMPUTE_MAX = 10000;
    if (dp_cap < PRECOMPUTE_MAX) init_dp_upto(PRECOMPUTE_MAX);
    if (dp_cap < n)              init_dp_upto(n);

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += dp[(size_t)d];
            idx_t e = n / d;
            if (e != d) sum += dp[(size_t)e];
        }
    }
    sum -= 1;                    /* exclude the trivial partition {n} */
    return sum;
}
