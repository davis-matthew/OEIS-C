/*
 * A326845  Sum times maximum of the integer partition with Heinz number n.
 * https://oeis.org/A326845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) * A061395(n)
 * Range:     u8 n <= 52, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A056239(n)  – sum of prime indices of the prime factors of n (with multiplicity) */
/*   A061395(n)  – maximum prime index among the prime factors of n (0 for n = 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static /* find 1‑based position of prime p in the sorted array primes[0..cnt-1] */
size_t prime_index(uint64_t p, const uint64_t *primes, size_t cnt)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;      /* 1‑based index */
        if (primes[mid] < p) lo = mid + 1; else hi = mid;
    }
    return 0;   /* should not happen for a valid prime factor */
}

seq_t a(idx_t n)
{
    /* static cache of primes up to the largest n seen so far */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt   = 0;
    static idx_t   prime_limit = 0;

    if (n < 1) return 0;

    /* Rebuild the prime list if necessary to cover the new n */
    if (n > prime_limit) {
        if (prime_list != NULL) free(prime_list);
        build_primes_upto((idx_t)n, &prime_list, &prime_cnt);
        prime_limit = n;
    }

    seq_t sum = 0;
    seq_t max = 0;
    uint64_t m = (uint64_t)n;

    /* Factorize m using the cached primes list */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m) break;
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t idx = (seq_t)(i + 1);            /* 1‑based prime index */
            sum += idx * (seq_t)exp;
            if (idx > max) max = idx;
        }
    }

    /* If a prime factor > sqrt(original n) remains */
    if (m > 1) {
        size_t idx = prime_index(m, prime_list, prime_cnt);
        if (idx == 0) return 0; /* should never happen */
        sum += (seq_t)idx;
        if ((seq_t)idx > max) max = (seq_t)idx;
    }

    return sum * max;
}
