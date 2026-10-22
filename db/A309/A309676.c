/*
 * A309676  Number of compositions (ordered partitions) of n into odd primes
 *          (including 1).
 * https://oeis.org/A309676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n using the part 1 or any odd
 *            prime. Recurrence: a(0)=1, a(n)=a(n-1)+∑_{p odd prime ≤ n}
 *            a(n-p)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   build_primes, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* static storage for primes up to the largest n requested */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    /* initialise cache with a(0)=1 */
    if (cache_len == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* rebuild prime list if we need primes beyond previous limit */
    if (n > prime_limit) {
        if (prime_list) free(prime_list);
        build_primes(n, (idx_t **)&prime_list, &prime_cnt);
        prime_limit = n;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_len) {
        grow_seq(&cache, &cache_cap, n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = cache[i - 1];               /* part 1 contributes a(i‑1) */
            for (size_t k = 0; k < prime_cnt; ++k) {
                uint64_t p = prime_list[k];
                if (p > i) break;
                if (p == 2) continue;              /* skip even prime 2 */
                sum += cache[i - (idx_t)p];
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
