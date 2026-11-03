/*
 * A292285  GCD of orders (in GF(p)) of primes dividing p-1, for odd primes p.
 * https://oeis.org/A292285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = GCD of orders (in GF(p)) of primes dividing p-1, for odd
 *            primes p
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9138 (2026-11-01)
 * Helpers:   factor_u64, gcd, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/powmod.h"

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

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Obtain the nth prime (1‑based).  For n=2 this gives p=3. */
    uint32_t p = get_prime(n);
    uint64_t p_minus1 = (uint64_t)p - 1ULL;

    /* Factor p-1.  primes[0..cnt-1] contain the distinct prime factors. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(p_minus1, primes, exps, &cnt);

    seq_t gcd_val = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t q = (seq_t)primes[i];                   /* the base for the order */
        seq_t ord = (seq_t)p_minus1;                  /* start with p-1 */

        /* Reduce ord to the multiplicative order of q mod p. */
        for (size_t j = 0; j < cnt; ++j)
        {
            seq_t r = (seq_t)primes[j];
            while (ord % r == 0)
            {
                seq_t candidate = ord / r;
                if (powmod(q, (uint64_t)candidate, (seq_t)p) == 1)
                    ord = candidate;
                else
                    break;
            }
        }

        if (gcd_val == 0)
            gcd_val = ord;
        else
            gcd_val = gcd(gcd_val, ord);
    }

    return gcd_val;
}
