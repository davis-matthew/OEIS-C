/*
 * A348323  Number of partitions of n into two or more primes.
 * https://oeis.org/A348323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into two or more primes
 * Range:     u8 n <= 38, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:  coin‑change dynamic program over the primes ≤ n.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 directly, as the empty partition is not allowed. */
    if (n == 0)
        return 0;

    /* Allocate DP table for partitions into primes. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return 0;                /* allocation failed – return 0 as a safe value */

    dp[0] = 1;                    /* one way to make 0 (empty sum) */

    /* Obtain all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)n, &primes, &prime_cnt);

    /* Standard coin‑change DP: each prime can be used any number of times. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* dp[n] counts all partitions into primes (including the single‑prime
       case).  Subtract that case if n itself is prime. */
    seq_t ans = dp[(size_t)n];
    if (is_prime(n))
        ans -= 1;

    /* Clean up. */
    free(dp);
    free(primes);
    return ans;
}
