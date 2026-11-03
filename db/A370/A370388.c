/*
 * A370388  First appearance of prime_i in A369797 or 0 if no such appearance
 *          occurs, with p_0 being 1. (A008578 but with a different offset.)
 * https://oeis.org/A370388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 * inv3) mod prime(n) for n > 2; * where inv3 is the
 *            modular inverse of 3 modulo prime(n). * Special cases: a(0) =
 *            10, a(1) = 6, a(2) = 0 * (the convention for p₀ is 1; primes for
 *            n ≥ 1 are standard primes)
 * Range:     u8 n <= 54, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

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
    /* Handle the few exceptional cases */
    if (n == 0)
        return (seq_t)10;
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)0;

    /* For n >= 3 we use the formula */
    uint64_t p = get_prime(n);          /* prime(n)  (n ≥ 3 → odd prime > 3) */
    uint64_t inv3;                      /* modular inverse of 3 mod p */

    /* Since p ≡ 1 or 2 (mod 3), the inverse is obtained without
       extended Euclid:
       - if p % 3 == 1:  inv3 = (2*p + 1) / 3
       - if p % 3 == 2:  inv3 = (p + 1) / 3
    */
    if (p % 3 == 1)
        inv3 = (2 * p + 1) / 3;
    else
        inv3 = (p + 1) / 3;

    /* Compute 2 * inv3 modulo p */
    seq_t val = ((seq_t)2 * (seq_t)inv3) % (seq_t)p;
    return val;
}
