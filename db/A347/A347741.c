/*
 * A347741  Number of compositions (ordered partitions) of n into at most 4
 *          prime parts.
 * https://oeis.org/A347741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 4 prime parts
 *            (offset 0: a(0)=1). This is computed by summing the number of
 *            ordered partitions of n into exactly k primes for k = 1..4
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* static cache of primes up to the largest n seen so far */
static uint64_t *prime_list = NULL;
static size_t prime_cnt     = 0;
static idx_t  prime_limit   = 0;

/* ensure that prime_list contains all primes <= limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* static DP buffers for consecutive k */
static seq_t *dp_prev = NULL;
static seq_t *dp_cur  = NULL;
static idx_t  dp_size = 0;

/* ensure DP buffers can hold indices up to n */
static void ensure_dp(idx_t n)
{
    idx_t needed = n + 1;
    if (dp_size >= needed)
        return;
    if (dp_prev) free(dp_prev);
    if (dp_cur)  free(dp_cur);
    dp_prev = (seq_t *)malloc(needed * sizeof(seq_t));
    dp_cur  = (seq_t *)malloc(needed * sizeof(seq_t));
    dp_size = needed;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == 0)
        return (seq_t)1;

    /* prepare data structures */
    ensure_primes(n);
    ensure_dp(n);

    /* initialize dp for k = 0 (empty sum) */
    for (idx_t i = 0; i <= n; ++i)
        dp_prev[i] = (seq_t)0;
    dp_prev[0] = (seq_t)1;

    seq_t ans = (seq_t)0;

    /* iterate k = 1..4 */
    for (idx_t k = 1; k <= 4; ++k) {
        /* zero current buffer */
        for (idx_t i = 0; i <= n; ++i)
            dp_cur[i] = (seq_t)0;

        /* convolution with all primes ≤ n */
        for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= n; ++pi) {
            idx_t p = prime_list[pi];
            idx_t max_s = n - p;
            for (idx_t s = 0; s <= max_s; ++s) {
                seq_t val = dp_prev[s];
                if (val != (seq_t)0)
                    dp_cur[s + p] += val;
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;

        /* accumulate compositions with exactly k parts */
        ans += dp_prev[n];
    }

    return ans;
}
