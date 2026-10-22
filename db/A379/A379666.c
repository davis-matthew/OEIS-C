/*
 * A379666  Array read by antidiagonals downward where A(n,k) is the number of
 *          integer partitions of n with product k.
 * https://oeis.org/A379666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of integer partitions of the sum with product,
 *            read by antidiagonals downward
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, index_to_sum_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/index_to_sum_prod.h"

/* Memoised dynamic‑programming table */
static seq_t  *dp_mem = NULL;
static idx_t  dp_stride = 0;
static idx_t  dp_cached_max_sum = 0;
static idx_t  dp_cached_max_prod = 0;

/* Rebuild the DP table up to given maximum sum and product. */
static void rebuild_dp(idx_t max_sum, idx_t max_prod)
{
    idx_t stride = max_prod + 1;
    size_t total = (size_t)(max_sum + 1) * (size_t)stride;
    seq_t *new_mem = alloc_seq_array(total);
    if (!new_mem) return;          /* allocation failed – keep old table */

    /* initialise to zero (alloc_seq_array may not zero‑init) */
    for (size_t i = 0; i < total; ++i) new_mem[i] = 0;

    /* base case: empty partition gives product 1 */
    new_mem[1] = 1;                /* dp[0][1] */

    for (idx_t part = 1; part <= max_sum; ++part) {
        for (idx_t s = part; s <= max_sum; ++s) {
            size_t s_base   = (size_t)s * stride;
            size_t prev_base = (size_t)(s - part) * stride;
            for (idx_t q = 1; q <= max_prod; ++q) {
                seq_t cur = new_mem[prev_base + q];
                if (!cur) continue;
                idx_t new_p = q * part;
                if (new_p > max_prod) continue;
                new_mem[s_base + new_p] += cur;
            }
        }
    }

    if (dp_mem) free_seq_array(dp_mem);
    dp_mem = new_mem;
    dp_stride = stride;
    dp_cached_max_sum = max_sum;
    dp_cached_max_prod = max_prod;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined below offset */

    idx_t sum, prod;
    index_to_sum_prod(n, &sum, &prod);

    /* extend DP table if necessary */
    if (sum > dp_cached_max_sum || prod > dp_cached_max_prod)
        rebuild_dp(sum > dp_cached_max_sum ? sum : dp_cached_max_sum,
                   prod > dp_cached_max_prod ? prod : dp_cached_max_prod);

    /* retrieve the stored value */
    size_t idx = (size_t)sum * dp_stride + prod;
    return dp_mem[idx];
}
