/*
 * A336469  a(n) = A329697(phi(n)), where A329697 is totally additive with
 *          a(2) = 0 and a(p) = 1 + a(p-1) for odd primes.
 * https://oeis.org/A336469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329697(phi(n)) where A329697 is defined by a(1) = 0,
 *            a(2) = 0, a(n) = 0 when n is a power of 2, for odd prime p: a(p)
 *            = 1 + a(p-1), for odd prime power p^e: a(p^e) = e * a(p), and a
 *            is totally additive: a(m*n)=a(m)+a(n) for coprime m, n. Here
 *            phi(n) is Euler's totient function
 * Range:     u8 n <= 821, u16 n <= 8907, u32 n <= 8907, u64 n <= 8907,
 *            u128 n <= 8907, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

#define CACHE_SIZE (1 << 20)   /* 1,048,576 – enough for phi(n) up to ~10^6 */

static seq_t a329_cache[CACHE_SIZE];
static bool  a329_seen [CACHE_SIZE];

/* forward declaration */
static seq_t a329_compute(seq_t m);

/* compute A329697(m) */
static seq_t a329_compute(seq_t m)
{
    /* cache hit? */
    if (m < CACHE_SIZE && a329_seen[(size_t)m])
        return a329_cache[(size_t)m];

    seq_t res = 0;

    if (m <= 1)
        res = 0;
    else if (m == 2)
        res = 0;
    else if ((m & (m - 1)) == 0)   /* m is power of 2 */
        res = 0;
    else if (is_prime((idx_t)m))   /* m is odd prime */
        res = 1 + a329_compute(m - 1); /* a(p) = 1 + a(p-1) */
    else                           /* composite > 2 */
    {
        uint64_t primes[64];
        uint64_t exps  [64];
        size_t   cnt   = 0;

        factor_u64((uint64_t)m, primes, exps, &cnt);

        for (size_t i = 0; i < cnt; ++i)
        {
            uint64_t p   = primes[i];
            uint64_t exp = exps[i];

            if (p == 2)            /* 2^e contributes 0 */
                continue;

            seq_t ap  = a329_compute((seq_t)p);   /* a(p) for odd prime p */
            res += (seq_t)exp * ap;               /* a(p^e) = e*a(p) */
        }
    }

    if (m < CACHE_SIZE)
    {
        a329_cache[(size_t)m] = res;
        a329_seen [(size_t)m] = true;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;    /* phi(1)=1, a(1)=A329697(1)=0 */

    /* compute phi(n) */
    seq_t phi = (seq_t)n;
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        phi /= (seq_t)p;
        phi *= (seq_t)(p - 1);
    }

    return a329_compute(phi);
}
