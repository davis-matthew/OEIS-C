/*
 * A380345  a(n) = n + sum of prime indices of n.
 * https://oeis.org/A380345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + sum of prime indices of n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/sieve_primes.h"   /* provides sieve_primes */
#include "../utils/factor_u64.h"     /* provides factor_u64 */

/* Global cache of primes for the thread of execution. */
static idx_t *primes = NULL;        /* array of prime values */
static size_t primes_cnt = 0;       /* number of primes in the array */
static idx_t sieve_limit = 0;       /* largest prime value that has been sieved */

/* Ensure primes up to at least 'limit' have been computed. */
static void ensure_sieve(idx_t limit) {
    if (sieve_limit >= limit)
        return;
    if (primes) {
        free(primes);
    }
    idx_t *tmp = NULL;
    size_t cnt = 0;
    sieve_primes(limit, &tmp, &cnt);
    primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    for (size_t i = 0; i < cnt; ++i) {
        primes[i] = tmp[i];
    }
    free(tmp);
    primes_cnt = cnt;
    sieve_limit = limit;
}

/* Return the 1‑based index of prime value 'p' in the global prime list.
   The function assumes p itself is prime.  If it is outside the current
   sieve, the sieve is extended. */
static idx_t prime_index(idx_t p) {
    if (p > sieve_limit) {
        /* Double the limit, with overflow protection. */
        idx_t new_limit = p * 2;
        if (new_limit < p)
            new_limit = SIZE_MAX;
        ensure_sieve(new_limit);
    }
    /* Binary search over the sorted prime array. */
    size_t left = 0, right = primes_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        idx_t v = primes[mid];
        if (v < p)
            left = mid + 1;
        else
            right = mid;
    }
    if (left < primes_cnt && primes[left] == p) {
        return (idx_t)(left + 1);   /* 1‑based index */
    }
    return 0; /* should never happen for a prime value */
}

/* Sequence a(n) = n + sum over the prime indices of n. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* safety for unexpected input */

    /* Initialise the prime list if this is the first call. */
    if (!primes) {
        /* For n up to ~10 000 the primes needed are < 130 000. */
        ensure_sieve((idx_t)(200000));   /* safe upper bound */
    }

    /* Factor n (n fits in idx_t, i.e. uint64_t). */
    uint64_t primes_local[64], exponents[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes_local, exponents, &pcnt);

    /* Sum of prime indices times their exponents. */
    idx_t sum = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = (idx_t)primes_local[i];
        idx_t idx = prime_index(p);
        sum += idx * (idx_t)exponents[i];
    }

    /* Result: n + sum, cast to seq_t. */
    seq_t result = (seq_t)n;
    result += (seq_t)sum;
    return result;
}
