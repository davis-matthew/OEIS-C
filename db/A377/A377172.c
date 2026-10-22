/*
 * A377172  Primes p such that -3/2 is a primitive root modulo p.
 * https://oeis.org/A377172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that -3/2 is a primitive root modulo p */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/powmod64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

static uint64_t *primes = NULL;          /* array of primes up to the current limit */
static size_t     primes_cnt = 0;        /* number of primes in the array */

/* term cache: stores the primes found so far that satisfy the condition */
static seq_t *terms   = NULL;
static size_t terms_cnt   = 0;
static size_t terms_cap   = 0;

/* index of the next prime to test */
static size_t prime_idx = 0;

/* prime limit used for the sieve */
static idx_t prime_limit = 500000; /* enough for n≈10000 */

/* make sure the prime array is initialized */
static void init_primes(void)
{
    if (primes != NULL)
        return;
    build_primes_upto(prime_limit, &primes, &primes_cnt);
}

/* test whether a is a primitive root modulo p (p is an odd prime) */
static bool is_primitive_root(uint64_t p, uint64_t a)
{
    uint64_t phi   = p - 1;
    uint64_t temp  = phi;
    /* factor phi using the primes array */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t q = primes[i];
        if (temp % q == 0) {
            if (powmod64(a, phi / q, p) == 1)
                return false;
            while (temp % q == 0)
                temp /= q;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        if (powmod64(a, phi / temp, p) == 1)
            return false;
    }
    return true;
}

/* append a new term to the terms cache */
static void append_term(seq_t val)
{
    if (terms_cnt == terms_cap) {
        /* grow capacity: double the size (minimum 1) */
        size_t new_cap = terms_cap ? terms_cap * 2 : 1;
        ensure_capacity(new_cap, &terms_cap, (void **)&terms, sizeof(seq_t));
    }
    terms[terms_cnt++] = val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_primes();

    while (terms_cnt < (size_t)n) {
        /* advance until we find a suitable prime */
        while (prime_idx < primes_cnt && terms_cnt < (size_t)n) {
            uint64_t p = primes[prime_idx++];
            if (p < 5) continue;      /* skip 2,3,5 etc. */

            /* compute (-3/2) mod p: inverse of 2 is (p+1)/2 */
            uint64_t inv2 = (p + 1) / 2;
            uint64_t a_val = ((p - 3) * inv2) % p;

            if (is_primitive_root(p, a_val))
                append_term((seq_t)p);
        }

        /* if we ran out of primes, extend the sieve */
        if (prime_idx >= primes_cnt && terms_cnt < (size_t)n) {
            /* double the limit */
            prime_limit *= 2;
            free(primes);
            primes = NULL;
            init_primes();
        }
    }

    return terms[(size_t)n - 1];
}
