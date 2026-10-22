/*
 * A362721  Number of numbers k, 1 <= k <= n, such that pi(k) = pi(n-k+1).
 * https://oeis.org/A362721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{k : 1 ≤ k ≤ n and pi(k) = pi(n-k+1)} * pi(x) = number
 *            of primes ≤ x, computed by a sieve
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* guard for the offset: a(0) is undefined, return 0 */
    if (n == 0) {
        return 0;
    }

    /* allocate array of booleans for the sieve */
    bool *is_prime = (bool*)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime)
        return 0;  /* allocation failure */

    /* initialise table: all true, then 0 and 1 are not prime */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    /* simple sieve of Eratosthenes */
    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                is_prime[m] = false;
        }
    }

    /* prefix prime counters in an idx_t array */
    idx_t *pi = (idx_t*)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!pi) {
        free(is_prime);
        return 0;
    }
    idx_t cnt = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (is_prime[i])
            ++cnt;
        pi[i] = cnt;
    }

    /* count values of k with equal prime counts */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t j = n - k + 1;
        if (pi[k] == pi[j])
            ++result;
    }

    free(is_prime);
    free(pi);
    return result;
}
