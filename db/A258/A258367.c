/*
 * A258367  a(n) is the smallest A (in absolute value) such that for p =
 *          prime(n), 2^{(p-1)/2} == +-1 + A*p (mod p^2), i.e., such that p is
 *          a near-Wieferich prime.
 * https://oeis.org/A258367
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(|A|, |p - A|) where p = prime(n) and 2^((p-1)/2) ≡ ±1
 *            + A⋅p (mod p²)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 7796, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9412 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* util helpers ---------------------------------------------------- */
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

/* sequence element ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset 2, below not used */

    /* ---------- 1. get the n‑th prime -------------------------------- */
    /* get_prime returns uint32_t, cast to seq_t to keep type uniform */
    seq_t p = (seq_t) get_prime(n);

    /* ---------- 2. compute 2^((p-1)/2)  (mod p²) ----------------------- */
    seq_t mod = p * p;                       /* p²  (fits in seq_t for the test sizes) */
    /* exponent is (p-1)/2 – fit into uint64_t because p ≤ 10⁶ approx */
    uint64_t exp = (uint64_t)((p - 1) / 2);
    seq_t S = powmod((seq_t)2, exp, mod);

    /* ---------- 3. find the unique integer A with 2^((p-1)/2) ≡ ±1 + A⋅p (mod p²) */
    seq_t r = S % p;               /* remainder of S modulo p */
    seq_t A;
    if (r == (seq_t)1) {                   /* S ≡ 1 (mod p) */
        A = (S - (seq_t)1) / p;
    } else if (r == p - (seq_t)1) {        /* S ≡ -1 (mod p) */
        A = (S + (seq_t)1) / p;
    } else {
        /* This case should never occur for a prime p.
           Return 0 as a safe fallback. */
        return 0;
    }

    /* ---------- 4. minimal absolute residue -------------------------- */
    seq_t diff = p - A;
    return (A <= diff) ? A : diff;
}
