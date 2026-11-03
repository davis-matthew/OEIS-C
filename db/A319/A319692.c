/*
 * A319692  a(n) = Product_{d|n, d<n} prime(1+A001414(d)), where A001414(d)
 *          gives the sum of prime factors of d, with repetition.
 * https://oeis.org/A319692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} A000040(1+A001414(d))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Validated: standard, pass, 81 terms through n = 7536 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A001414(d) is the sum of the prime factors of d (with multiplicity). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper declarations – the build will provide the required headers. */
#include <stdlib.h>
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

/* ------------------------------------------------------------------
 * sum_prime_factors
 *
 * Compute the sum of the prime factors of x, counting multiplicity.
 * For x <= 1 the sum is 0.
 * ------------------------------------------------------------------ */
static seq_t
sum_prime_factors(idx_t x)
{
    if (x <= 1)
        return (seq_t)0;

    /* The number of distinct prime factors of numbers up to 10 000 is < 10. */
    uint64_t primes[16], exps[16];
    size_t cnt = 0;

    factor_u64((uint64_t)x, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* primes[i] and exps[i] fit in 64 bits; their product in seq_t. */
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }
    return sum;
}

/* ------------------------------------------------------------------
 * a
 *
 * Main entry point for OEIS sequence A319692.
 * ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* a(1) = 1 */

    /* Gather proper divisors of n (excluding n itself). */
    idx_t divisors[256];
    size_t dcnt = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 < n)
                divisors[dcnt++] = d1;
            if (d2 < n && d2 != d1)
                divisors[dcnt++] = d2;
        }
    }

    /* Compute the product of the required primes. */
    seq_t prod = (seq_t)1;
    for (size_t k = 0; k < dcnt; ++k) {
        idx_t d = divisors[k];
        seq_t sum = sum_prime_factors(d);           /* A001414(d) */
        idx_t prime_index = 1 + (idx_t)sum;          /* 1‑based index */
        seq_t prime_val   = (seq_t)get_prime(prime_index);
        prod *= prime_val;
    }

    return prod;
}
