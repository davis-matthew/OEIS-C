/*
 * A324967  Number of distinct even prime indices of n.
 * https://oeis.org/A324967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime divisors of n whose prime index
 *            is even (equivalently a(n) = A001221(n) - A324966(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* static state for prime list */
static uint64_t *PrimeList   = NULL;   /* array of primes up to current limit */
static size_t   PrimeCount  = 0;       /* number of primes stored */
static idx_t    PrimeLimit  = 0;       /* maximum n for which primes have been generated */

/* Ensure we have primes up to at least 'limit'.  Rebuild the list if necessary. */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= PrimeLimit)
        return;
    /* free old list if any */
    if (PrimeList != NULL)
        free(PrimeList);
    /* build new list up to 'limit' */
    build_primes_upto(limit, &PrimeList, &PrimeCount);
    PrimeLimit = limit;
}

/* Binary search for the 1‑based position of prime 'p' in PrimeList. */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = PrimeCount;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t val = PrimeList[mid];
        if (val == p)
            return (idx_t)(mid + 1);          /* 1‑based index */
        if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* should never happen, p is guaranteed to be prime in the list */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* special cases: n < 2 has no prime divisors */
    if (n < 2)
        return 0;

    /* Ensure primes are available up to n */
    ensure_prime_limit(n);

    /* factor n into distinct prime divisors */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* count distinct primes with even prime index */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        idx_t idx = prime_index(p);
        if ((idx & 1) == 0)   /* even index */
            ++res;
    }
    return res;
}
