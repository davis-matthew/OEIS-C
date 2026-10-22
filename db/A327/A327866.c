/*
 * A327866  a(n) = 1 if arithmetic derivative of n is square, 0 otherwise. Cf.
 *          A003415.
 * https://oeis.org/A327866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is a perfect square,
 *            0 otherwise. The derivative D(n) = n * Σ_{p|n} (e_p / p), where
 *            e_p is the exponent of prime p in the factorisation of n. D(n)
 *            can also be computed as Σ_{p^e‖n} e * n / p
 * Range:     u8 n <= 31, u16 n <= 8798, u32 n <= 9296, u64 n <= 9296,
 *            u128 n <= 9296, big any n
 * Helpers:   build_primes_upto, floor_sqrt, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/int_sqrt.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;     /* prime list created on demand */
static size_t prime_cnt  = 0;        /* number of primes stored */
static idx_t prime_limit = 0;       /* highest value for which primes were built */

/* Build or extend the prime table up to at least 'limit'. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free any previous allocation */
    if (primes) { free(primes); primes = NULL; }
    prime_cnt = 0;

    /* create new prime list up to 'limit' */
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Return 1 if x is a perfect square, 0 otherwise. */
static seq_t is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return (s * s == x) ? 1 : 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The arithmetic derivative of 0 and 1 is 0, which is a perfect square. */
    if (n <= 1) return 1;

    seq_t D = 0;           /* accumulator for D(n) */
    idx_t m = n;           /* factorised form of n */

    /* Ensure we have primes up to sqrt(n). */
    idx_t sqrt_n = int_sqrt(n);
    ensure_primes(sqrt_n);

    /* Factorise n using the prime table. */
    for (size_t i = 0; i < prime_cnt && primes[i] <= sqrt_n; ++i)
    {
        uint64_t p = primes[i];
        if (m % p == 0)
        {
            size_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            /* contribute e * n / p, computed safely using 128‑bit intermediate. */
            seq_t contrib = (seq_t)((unsigned __int128)(n) / p * e);
            D += contrib;
        }
    }

    /* If n has a remaining prime factor larger than sqrt(n). */
    if (m > 1)
    {
        uint64_t p = (uint64_t)m;
        seq_t contrib = (seq_t)((unsigned __int128)(n) / p);
        D += contrib;
    }

    /* Check whether D(n) is a perfect square. */
    return is_square(D);
}
