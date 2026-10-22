/*
 * A347552  Number of partitions of n into at most 2 prime parts.
 * https://oeis.org/A347552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into at most 2 prime parts. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle the offset terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Static cache of primes up to the largest n requested so far. */
    static uint64_t *primes   = NULL;   /* array of primes */
    static size_t   prime_cnt = 0;      /* number of primes stored */
    static idx_t    primes_limit = 0;   /* largest n for which primes are up to date */

    /* If we need primes beyond the current cache, rebuild. */
    if (primes == NULL || primes_limit < n) {
        free(primes);                        /* free old array, if any */
        build_primes_upto(n, &primes, &prime_cnt);
        primes_limit = n;
    }

    seq_t ans = 0;

    /* Count the single-prime partition, if n itself is prime. */
    if (n >= 2 && is_prime(n))
        ans += (seq_t)1;

    /* Count unordered pairs of primes that sum to n. */
    idx_t half = n / 2;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p > half) break;            /* primes are in ascending order */
        idx_t q = n - p;
        if (is_prime(q))
            ans += (seq_t)1;
    }

    return ans;
}
