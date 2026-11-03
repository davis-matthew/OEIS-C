/*
 * A392859  Multiplicative inverse of {A143293(n) modulo prime(1+n)} in the
 *          finite field F(prime(1+n)), or 0 if prime(1+n) divides A143293(n),
 *          where A143293 gives the partial sums of primorials.
 * https://oeis.org/A392859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A143293(n))^{-1} (mod prime(n+1)), where A143293(n) =
 *            sum_{k=0}^n primorial_k, primorial_0 = 1, primorial_k (k>0) =
 *            primorial_{k-1} * prime_k. If prime(n+1) divides A143293(n),
 *            then a(n) = 0
 * Range:     u8 n <= 53, u16 n <= 4827, u32 n <= 6719, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9023 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
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
    /* prime_{n+1} */
    seq_t prime = (seq_t)get_prime(n + 1);

    /* partial sum of primorials modulo prime */
    seq_t sum  = ((seq_t)1) % prime;   /* primorial_0 = 1 */
    seq_t prod = ((seq_t)1) % prime;   /* running product for primorial */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t p = (seq_t)get_prime(i);       /* prime_i   (1‑indexed) */
        prod = (prod * p) % prime;           /* primorial_i */
        sum  = (sum + prod) % prime;         /* add to partial sum */
    }

    if (sum == (seq_t)0)
        return (seq_t)0;                     /* division by zero case */

    /* Fermat's little theorem to compute the inverse modulo a prime */
    uint64_t exp = (uint64_t)(prime - (seq_t)2);
    return powmod(sum, exp, prime);
}
