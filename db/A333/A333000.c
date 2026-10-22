/*
 * A333000  Sum of integers (with multiplicity) encountered on all possible
 *          paths from n to 1 when iterating with nondeterministic map k -> k-
 *          k/p, where p is any prime factor of k.
 * https://oeis.org/A333000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A333123(n) + Sum_{p|n, p prime} a(n - n/p) where
 *            A333123(n) is the number of paths from n to 1, with the base
 *            cases a(1) = 1 and A333123(1) = 1
 * Range:     u8 n <= 18, u16 n <= 128, u32 n <= 2880, u64 n <= 9539,
 *            u128 n <= 9539, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t *counts = NULL;      /* A333123(n)  */
static seq_t *sums   = NULL;      /* a(n)        */
static idx_t computed_max = 0;    /* highest n computed */
static size_t cache_cap   = 0;    /* allocated size (max index) */

static void ensure_cache(idx_t up_to)
{
    if (up_to <= computed_max) return;

    /* Grow storage if necessary */
    if (up_to > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)up_to) new_cap <<= 1;

        seq_t *new_counts = (seq_t *)realloc(counts,
                                             sizeof(seq_t) * (new_cap + 1));
        seq_t *new_sums   = (seq_t *)realloc(sums,
                                             sizeof(seq_t) * (new_cap + 1));
        if (!new_counts || !new_sums) exit(EXIT_FAILURE);

        /* Initialise new slots to zero */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
            new_counts[i] = 0;
            new_sums[i]   = 0;
        }
        counts = new_counts;
        sums   = new_sums;
        cache_cap = new_cap;
    }

    /* Compute values from computed_max+1 up to up_to */
    for (idx_t k = computed_max + 1; k <= up_to; ++k) {
        if (k == 1) {              /* base cases */
            counts[1] = 1;
            sums[1]   = 1;
            continue;
        }

        /* Factor k to get its distinct prime divisors */
        uint64_t primes[64], exps[64];
        size_t prcnt;
        factor_u64((uint64_t)k, primes, exps, &prcnt);

        seq_t cnt = 0;
        seq_t sum = 0;
        for (size_t i = 0; i < prcnt; ++i) {
            uint64_t p = primes[i];
            idx_t next = k - k / p;      /* k - k/p   */
            cnt += counts[next];
            sum += sums[next];
        }

        counts[k] = cnt;
        sums[k]   = (seq_t)k * cnt + sum;
    }

    computed_max = up_to;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return sums[n];
}
