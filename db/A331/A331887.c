/*
 * A331887  Number of partitions of n into distinct parts having a common
 *          factor > 1 with n.
 * https://oeis.org/A331887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k: gcd(n,k)>1} (1 + x^k)
 * Range:     u8 n <= 47, u16 n <= 119, u32 n <= 329, u64 n <= 1049,
 *            u128 n <= 3779, big any n
 * Helpers:   get_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_distinct_prime_factors.h"

/* Count partitions of n into distinct parts all sharing a common factor > 1 with n. */
static bool is_prime_factor_set = false;    /* unused – kept for future extension */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) = 1 by definition */
    if (n == 1) return (seq_t)0;  /* a(1) = 0 */

    /* Allocate arrays for allowed parts and DP */
    size_t size = (size_t)n + 1;
    bool *allow = (bool*)calloc(size, sizeof(bool));
    if (!allow) return (seq_t)0;  /* allocation failure */

    /* Find distinct prime factors of n */
    idx_t factors[64];
    size_t fc;
    get_distinct_prime_factors(n, factors, &fc);

    /* Mark all multiples of the prime factors as allowed parts */
    for (size_t i = 0; i < fc; ++i) {
        idx_t p = factors[i];
        for (idx_t m = p; m <= n; m += p) {
            allow[m] = true;
        }
    }

    /* DP array for partitions into distinct allowed parts */
    seq_t *dp = (seq_t*)calloc(size, sizeof(seq_t));
    if (!dp) {
        free(allow);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;  /* empty partition */

    for (idx_t k = 1; k <= n; ++k) {
        if (!allow[k]) continue;
        for (idx_t s = n; s >= k; --s) {
            dp[s] += dp[s - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(allow);
    return result;
}
