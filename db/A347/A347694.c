/*
 * A347694  Largest k <= 2*n such that k is a prime or twice a prime.
 * https://oeis.org/A347694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A347694: a(n) = largest k ≤ 2*n such that k is prime or k = 2*p with p prime */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Static cache of primes up to the largest requested limit. */
static idx_t _prime_limit = 0;
static uint64_t *_primes = NULL;
static size_t _prime_cnt = 0;

/* Ensure that primes up to 'limit' are available. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= _prime_limit)
        return;
    /* Build primes up to 'limit', replacing old array. */
    uint64_t *pr = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &pr, &cnt);
    /* Free previous array if any. */
    if (_primes != NULL)
        free(_primes);
    _primes = pr;
    _prime_cnt = cnt;
    _prime_limit = limit;
}

/* Return the largest prime ≤ N, or 0 if none. */
static uint64_t _prev_prime(idx_t N)
{
    if (N < 2)
        return 0;
    for (size_t i = _prime_cnt; i-- > 0; ) {
        uint64_t p = _primes[i];
        if (p <= N)
            return p;
    }
    return 0; /* Should not happen for N ≥ 2. */
}

seq_t a(idx_t n)
{
    /* Problem definition only specifies n ≥ 1; behaviour for n = 0 is irrelevant. */
    if (n == 0u) return 0;

    /* Upper bound 2*n. */
    idx_t limit = (idx_t)(2u * (seq_t)n);
    _ensure_primes(limit);

    uint64_t p = _prev_prime(limit);   /* largest prime ≤ 2*n      */
    uint64_t q = _prev_prime(n);       /* largest prime ≤ n        */

    seq_t cand1 = (seq_t)p;           /* k = p                     */
    seq_t cand2 = (seq_t)(q * 2u);     /* k = 2*q                   */

    return (cand1 > cand2) ? cand1 : cand2;
}
