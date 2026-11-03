/*
 * A318885  If n = p^a * q^b * ... * r^c, with p < q < r primes, with nonzero
 *          exponents a, b, c, then a(n) = prime(1+p-p)^a * prime(1+q-p)^b *
 *          ... * prime(1+r-p)^c; a(1) = 1.
 * https://oeis.org/A318885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ over primes p of n ( prime(1+p-p₁) )^e_p with p₁ =
 *            smallest prime divisor of n where n = ∏ p^e_p, p₁ < … < p. For
 *            n=1, a(1)=1
 * Range:     u8 n <= 73, u16 n <= 3717, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 105 terms through n = 9878 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper routines provided by the build environment.                 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Compute the A318885 sequence term for index n (n ≥ 1).              */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Allocate buffers large enough to hold the prime factorisation of n. */
    uint64_t primes[64] = {0};
    uint64_t exps[64]   = {0};
    size_t   cnt        = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* The smallest prime factor is primes[0].                             */
    uint64_t p1 = primes[0];
    seq_t    res = ipow((seq_t)2, (seq_t)exps[0]); /* prime(1)=2 */

    /* Process the remaining prime factors.                               */
    for (size_t i = 1; i < cnt; ++i)
    {
        uint64_t diff = primes[i] - p1;                 /* p - p₁          */
        idx_t    idx  = (idx_t)(1 + diff);              /* index of prime   */
        seq_t    w    = ipow((seq_t)get_prime(idx), (seq_t)exps[i]);
        res *= w;
    }

    return res;
}
