/*
 * A341656  a(n) is the number of divisors of prime(n)^4 - 1.
 * https://oeis.org/A341656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of prime(n)^4 – 1
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the identity
   prime^4 – 1 = (prime – 1)(prime + 1)(prime^2 + 1)
   and factorises each 64‑bit component with factor_u64.
   The total divisor count is the product of (exponent + 1).  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

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

static void merge_factor(uint64_t prime, uint64_t exp,
                        uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t i;
    for (i = 0; i < *cnt; ++i) {
        if (primes[i] == prime) {
            exps[i] += exp;
            return;
        }
    }
    /* new prime */
    primes[*cnt] = prime;
    exps[*cnt] = exp;
    (*cnt)++;
}

seq_t a(idx_t n)
{
    /* obtain the n-th prime (1‑based) */
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;

    /* components of p^4 – 1 */
    uint64_t b1 = p * p + 1;   /* p^2 + 1 */
    uint64_t b2 = p - 1;       /* p – 1 */
    uint64_t b3 = p + 1;       /* p + 1 */

    /* arrays to hold aggregated factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    /* factor each component and merge */
    {
        size_t cnt;
        uint64_t pr[64], ex[64];

        factor_u64(b1, pr, ex, &cnt);
        for (size_t j = 0; j < cnt; ++j) {
            merge_factor(pr[j], ex[j], primes, exps, &count);
        }
    }
    {
        size_t cnt;
        uint64_t pr[64], ex[64];

        factor_u64(b2, pr, ex, &cnt);
        for (size_t j = 0; j < cnt; ++j) {
            merge_factor(pr[j], ex[j], primes, exps, &count);
        }
    }
    {
        size_t cnt;
        uint64_t pr[64], ex[64];

        factor_u64(b3, pr, ex, &cnt);
        for (size_t j = 0; j < cnt; ++j) {
            merge_factor(pr[j], ex[j], primes, exps, &count);
        }
    }

    /* compute divisor count as product of (exponent+1) */
    seq_t result = 1;
    for (size_t i = 0; i < count; ++i) {
        result *= (seq_t)(exps[i] + 1);
    }

    return result;
}
