/*
 * A372000  a(n) = product of primes p such that floor(n/p) is odd.
 * https://oeis.org/A372000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of all primes p such that floor(n/p) is odd
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 68, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple incremental sieve to generate all primes up
   to the requested n.  It works for any build where seq_t is an
   integer type or an arbitrary‑precision integer class. */
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* integer square root for unsigned 64‑bit values */
static inline idx_t int_sqrt(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* data kept between calls to a() */
static struct {
    idx_t      limit;       /* highest index already sieved */
    bool      *is_comp;     /* composite flag array, size limit+1 */
    uint64_t  *primes;      /* array of primes found so far */
    size_t    primes_cnt;   /* number of primes stored */
} prime_sieve = {0, NULL, NULL, 0};

/* extend the sieve so that all primes up to n are known */
static void ensure_primes(idx_t n) {
    if (n <= prime_sieve.limit)
        return;                       /* nothing to do */

    idx_t old_limit = prime_sieve.limit;
    prime_sieve.limit = n;

    /* resize the composite flag array */
    bool *tmp = (bool*)realloc(prime_sieve.is_comp,
                               (size_t)(prime_sieve.limit + 1));
    if (tmp == NULL) abort();          /* allocation failure */
    prime_sieve.is_comp = tmp;
    for (idx_t i = old_limit + 1; i <= prime_sieve.limit; ++i)
        prime_sieve.is_comp[i] = false;

    /* mark composites in the new range using the primes we already have */
    idx_t sqrt_n = int_sqrt(prime_sieve.limit);
    for (size_t i = 0; i < prime_sieve.primes_cnt; ++i) {
        uint64_t p = prime_sieve.primes[i];
        if (p > (uint64_t)sqrt_n)
            break;
        uint64_t start;
        if (old_limit < (uint64_t)p * p)
            start = (uint64_t)p * p;
        else
            start = ((old_limit + p) / p) * p;
        for (uint64_t j = start; j <= prime_sieve.limit; j += p)
            prime_sieve.is_comp[j] = true;
    }

    /* identify any new primes in the extended interval */
    for (idx_t x = (old_limit < 2 ? 2 : old_limit + 1); x <= prime_sieve.limit; ++x) {
        if (!prime_sieve.is_comp[x]) {
            uint64_t *newp = (uint64_t*)realloc(prime_sieve.primes,
                                                (size_t)(prime_sieve.primes_cnt + 1)
                                                * sizeof(uint64_t));
            if (newp == NULL) abort();  /* allocation failure */
            prime_sieve.primes = newp;
            prime_sieve.primes[prime_sieve.primes_cnt++] = (uint64_t)x;
        }
    }
}

seq_t a(idx_t n) {
    assert(n >= 1);                     /* valid for n >= 1 only */

    ensure_primes(n);

    seq_t result = 1;
    for (size_t i = 0; i < prime_sieve.primes_cnt; ++i) {
        uint64_t p = prime_sieve.primes[i];
        if (p > (uint64_t)n)
            break;
        idx_t t = n / p;                /* floor(n/p) */
        if (t & 1)                      /* odd? */
            result *= (seq_t)p;
    }
    return result;
}
