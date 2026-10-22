/*
 * A325195  Difference between the length of the minimal triangular partition
 *          containing and the maximal triangular partition contained in the
 *          Young diagram of the integer partition with Heinz number n.
 * https://oeis.org/A325195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325195: a(n) = minimal triangular partition length containing
   the partition with Heinz number n minus maximal triangular
   partition length contained in that Young diagram. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"          /* factor_u64 */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

/* Comparison function for qsort: descending order of parts. */
static int cmp_desc(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t *)p1;
    idx_t b = *(const idx_t *)p2;
    return (a > b) ? -1 : (a < b) ? 1 : 0;
}

/* Dynamically kept list of primes up to prime_list_limit. */
static uint64_t *prime_list = NULL;
static size_t     prime_count = 0;
static idx_t      prime_limit = 0;

/* Make sure prime_list contains all primes <= limit. */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;                       /* no need to rebuild */

    uint64_t *new_list = NULL;
    size_t   new_count = 0;
    build_primes_upto(limit, &new_list, &new_count);

    free(prime_list);
    prime_list  = new_list;
    prime_count = new_count;
    prime_limit = limit;
}

/* Return 1‑based index of prime p among primes <= p. */
static size_t prime_index(uint64_t p)
{
    ensure_prime_limit(p);
    /* binary search for first > p, answer is count of primes <= p */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo; /* 1‑based */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)0;                 /* empty partition */

    /* Factorize n into primes and exponents. */
    uint64_t primes[64], exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt == 0)                          /* n == 1 */
        return (seq_t)0;

    /* Find maximum prime factor to build prime list once. */
    uint64_t maxPrime = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] > maxPrime)
            maxPrime = primes[i];

    ensure_prime_limit(maxPrime);           /* ensure prime list ready */

    /* Compute total number of parts (sum of exponents). */
    idx_t k = 0;
    for (size_t i = 0; i < cnt; ++i)
        k += (idx_t)exps[i];

    if (k == 0)
        return (seq_t)0;

    /* Allocate array for parts. */
    idx_t *parts = (idx_t *)malloc(sizeof(idx_t) * k);
    if (!parts)
        return (seq_t)0;                     /* out of memory */

    /* Fill parts with prime indices, repeated by exponent. */
    idx_t pos = 0;
    for (size_t pi = 0; pi < cnt; ++pi) {
        uint64_t prime = primes[pi];
        uint64_t exp   = exps[pi];
        size_t   idx   = prime_index(prime); /* 1‑based index */

        for (uint64_t j = 0; j < exp; ++j)
            parts[pos++] = (idx_t)idx;
    }

    /* Sort parts in descending order. */
    if (k > 1)
        qsort(parts, k, sizeof(idx_t), cmp_desc);

    /* Compute A_i = λ_i + i (0‑based). Find min and max. */
    idx_t minA = (idx_t)-1;
    idx_t maxA = 0;
    for (idx_t i = 0; i < k; ++i) {
        idx_t Ai = parts[i] + i;
        if (Ai < minA) minA = Ai;
        if (Ai > maxA) maxA = Ai;
    }

    /* t_min = maxA */
    /* t_max = min(k, minA) */
    idx_t t_min = maxA;
    idx_t t_max = (minA <= k) ? minA : k;

    free(parts);

    return (seq_t)(t_min - t_max);
}
