/*
 * A367095  Number of distinct sums of pairs (repeats allowed) of prime
 *          indices of n.
 * https://oeis.org/A367095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums of pairs (repeats allowed) of
 *            prime indices of n
 * Range:     u8 n <= 9983, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/compute_pi.h"

static uint64_t *sieve_primes = NULL;     /* primes up to a fixed limit */
static size_t   sieve_count = 0;

/* comparison function for qsort on idx_t */
static int cmp_uint64(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a, *y = (const idx_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Build a static sieve of primes up to 100000 (enough for n up to ~10^10) */
    if (!sieve_primes) {
        const uint64_t SIEVE_LIMIT = 100000;
        build_primes_upto(SIEVE_LIMIT, &sieve_primes, &sieve_count);
    }

    /* collect distinct prime indices */
    enum { MAX_FACTORS = 64 };
    idx_t prime_indices[MAX_FACTORS];
    size_t k = 0;

    uint64_t tmp = (uint64_t)n;
    for (size_t i = 0; i < sieve_count && sieve_primes[i] * sieve_primes[i] <= tmp; ++i) {
        uint64_t p = sieve_primes[i];
        if (tmp % p == 0) {
            prime_indices[k++] = (idx_t)(i + 1);   /* prime indices start at 1 */
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1) {                      /* remaining prime factor > sieve limit */
        prime_indices[k++] = (idx_t)compute_pi(tmp);
    }

    if (k == 0)
        return (seq_t)0;

    /* generate all sums of pairs (i,j) with i <= j */
    enum { MAX_SUMS = 2100 };           /* enough for up to 64 distinct factors */
    idx_t sums[MAX_SUMS];
    size_t sum_cnt = 0;

    for (size_t i = 0; i < k; ++i) {
        for (size_t j = i; j < k; ++j) {
            sums[sum_cnt++] = prime_indices[i] + prime_indices[j];
        }
    }

    /* sort sums and count distinct values */
    qsort(sums, sum_cnt, sizeof(idx_t), cmp_uint64);

    size_t distinct = 1;
    for (size_t i = 1; i < sum_cnt; ++i) {
        if (sums[i] != sums[i - 1])
            ++distinct;
    }

    return (seq_t)distinct;
}
