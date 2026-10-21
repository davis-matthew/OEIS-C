/*
 * A295424  Number of distinct twin primes which are in Goldbach partitions of
 *          2n.
 * https://oeis.org/A295424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct twin primes occurring in any Goldbach
 *            partition of 2n. The function enumerates all Goldbach partitions
 *            of 2n, collects the primes involved, checks which of them belong
 *            to a twin prime pair, and counts the distinct such primes
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
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

static idx_t  cache_max_n = 0;
static uint64_t *cache_primes = NULL;
static size_t   cache_prime_cnt = 0;

seq_t a(idx_t n)
{
    /* Handle trivial cases quickly. */
    if (n < 3) return 0;

    /* Extend prime cache if necessary. */
    if (n > cache_max_n) {
        if (cache_primes) free(cache_primes);
        build_primes_upto( (2*n), &cache_primes, &cache_prime_cnt );
        cache_max_n = n;
    }

    idx_t limit = 2*n;
    /* Mark primes up to 2n. */
    uint8_t *prime_flag = (uint8_t*)calloc(limit + 1, 1);
    for (size_t i = 0; i < cache_prime_cnt; ++i) {
        uint64_t p = cache_primes[i];
        if (p <= limit) prime_flag[p] = 1;
    }

    /* Mark twin primes that appear in any Goldbach partition. */
    uint8_t *twin_used = (uint8_t*)calloc(limit + 1, 1);

    for (size_t i = 0; i < cache_prime_cnt; ++i) {
        uint64_t p = cache_primes[i];
        if (p > n) break;          /* need q = 2n - p >= p */
        uint64_t q = (uint64_t)limit - p;
        if (q > limit) continue;   /* safety check */
        if (!prime_flag[q]) continue;   /* not a Goldbach partition */

        /* For prime p */
        if ((p >= 2 && prime_flag[p-2]) || (p+2 <= limit && prime_flag[p+2]))
            twin_used[p] = 1;

        /* For prime q (might equal p) */
        if (q != p) {
            if ((q >= 2 && prime_flag[q-2]) || (q+2 <= limit && prime_flag[q+2]))
                twin_used[q] = 1;
        }
    }

    /* Count distinct twin primes used. */
    seq_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (twin_used[i]) ++count;
    }

    /* Clean up. */
    free(prime_flag);
    free(twin_used);

    return count;
}
