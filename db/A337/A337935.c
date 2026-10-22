/*
 * A337935  Numbers with integer contraharmonic mean of distinct prime
 *          factors.
 * https://oeis.org/A337935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337935:  a(n) = the n-th integer m such that the
 * contraharmonic mean of the distinct prime factors of m is an integer.
 * The contraharmonic mean of primes p1,p2,…,pk is (p1²+…+pk²)/(p1+…+pk).
 * m is included in the sequence if that fraction is an integer.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t, uint64_t**,size_t*) */
#include "../utils/grow_seq.h"           /* grow_seq(seq_t**,size_t*,size_t) */

/* Cached data for successive calls */
static seq_t *cache = NULL;              /* terms already found */
static size_t cache_len = 0;             /* number of terms stored */
static size_t cache_cap = 0;             /* allocated capacity for cache */

static uint64_t *primes = NULL;          /* list of primes up to prime_limit */
static size_t primes_cnt = 0;            /* number of primes in the list */
static size_t prime_limit = 0;           /* largest prime covering all primes used */

/* Ensure we have primes up to at least limit.  The array is rebuilt only
 * when the required limit exceeds the current limit, growing by a factor
 * of two each time to keep the number of rebuilds logarithmic. */
static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= prime_limit)
        return;

    /* grow the limit until it is enough */
    size_t new_limit = prime_limit ? prime_limit : 4;
    while (new_limit < limit)
        new_limit <<= 1;   /* double until sufficient */

    /* free old list and build a new one */
    if (primes)
        free(primes);
    build_primes_upto(new_limit, &primes, &primes_cnt);
    prime_limit = new_limit;
}

/* Factorize x into distinct primes using the list of primes.
 * sum_pr   accumulates the sum of the distinct primes.
 * sum_sq   accumulates the sum of the squares of the distinct primes. */
static void factor_into_sums(uint64_t x, uint64_t *sum_pr, uint64_t *sum_sq) {
    *sum_pr = 0;
    *sum_sq = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x)
            break;
        if (x % p == 0) {
            *sum_pr += p;
            *sum_sq += p * p;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1) {           /* remaining prime factor */
        *sum_pr += x;
        *sum_sq += x * x;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;           /* undefined; guard against unsafe access */

    static uint64_t checked = 1;    /* largest integer examined so far */

    /* Ensure enough terms are cached */
    while (cache_len < n) {
        ++checked;

        /* Ensure we have enough primes for factorisation */
        ensure_primes_up_to((checked > 1) ? checked : 2);

        uint64_t sum_pr, sum_sq;
        factor_into_sums(checked, &sum_pr, &sum_sq);

        /* Test integer contraharmonic mean */
        if (sum_pr != 0 && (sum_sq % sum_pr) == 0) {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                grow_seq(&cache, &cache_cap, new_cap);
            }
            cache[cache_len++] = (seq_t)checked;
        }
    }

    return cache[n - 1];
}
