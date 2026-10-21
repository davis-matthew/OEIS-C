/*
 * A295470  a(n) = (20*n)!*(6*n)!*n!/((12*n)!*(10*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*n)!*(6*n)!*n! / ((12*n)!*(10*n)!*(3*n)!*(2*n)!)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies from the utils library */
#include <stdlib.h>            /* for free() */
#include <stddef.h>            /* for size_t */
#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Static state for prime caching */
static idx_t *primes = NULL;   /* array of primes up to current limit */
static size_t prime_count = 0; /* number of primes stored */
static idx_t prime_limit = 0;  /* current maximum prime value stored */

/* Compute exponent of prime 'p' in m! via Legendre's formula */
static idx_t legendre(idx_t m, idx_t p)
{
    idx_t exp = 0;
    for (idx_t pk = p; pk <= m; pk *= p)
    {
        exp += m / pk;
        /* Prevent overflow of pk *= p */
        if (pk > m / p) break;
    }
    return exp;
}

/* Ensure we have all primes up to 'limit' (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Free existing list if any */
    if (primes != NULL)
        free(primes);

    /* Build primes up to 'limit' */
    build_primes(limit, (idx_t **)&primes, &prime_count);
    prime_limit = limit;
}

/* Compute a(n) for the OEIS sequence A295470 */
seq_t a(idx_t n)
{
    /* We need primes up to 20*n for the factorials involved. */
    idx_t up = 20 * n;
    ensure_primes(up);

    seq_t res = (seq_t)1;  /* accumulator */

    for (size_t i = 0; i < prime_count; ++i)
    {
        idx_t p = primes[i];
        if (p > up)
            break;

        /* exponent of p in the product formula */
        idx_t e = legendre(20 * n, p)
                + legendre( 6 * n, p)
                + legendre(  n, p)
                - legendre(12 * n, p)
                - legendre(10 * n, p)
                - legendre( 3 * n, p)
                - legendre( 2 * n, p);

        if (e > 0)
        {
            seq_t part = seq_pow((seq_t)p, e);  /* p^e */
            res = seq_mul(res, part);           /* multiply into result */
        }
    }

    return res;
}
