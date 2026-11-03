/*
 * A334904  a(n) is the least integer b such that the fractions (b^0)/p,
 *          (b^1)/p, ..., (b^(r-1))/p where p is the n-th prime, produce the
 *          A006556(n) distinct cycles.
 * https://oeis.org/A334904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal b < p_n such that the fractions * b^0/p_n,
 *            b^1/p_n, … , b^(r-1)/p_n generate the r distinct decimal * digit
 *            cycles for the n-th prime p_n. Here r = (p_n-1)/ord_p(10)
 * Range:     u8 n <= 54, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 92 (2026-11-01)
 * Helpers:   gcd, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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

/* small helper: factorize an unsigned 64‑bit number into distinct prime factors.
 * The result is stored in `pf` and the number of distinct primes is returned. */
static size_t _factorize(uint64_t n, uint64_t *pf, size_t max_pf)
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n && cnt < max_pf; ++p) {
        if (n % p == 0) {
            pf[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1 && cnt < max_pf) pf[cnt++] = n;
    return cnt;
}

/* compute the multiplicative order of `base` modulo `mod`.
 * The function receives the distinct prime factors of `mod-1`. */
static seq_t _order(seq_t base, seq_t mod, uint64_t *pf, size_t pf_cnt)
{
    seq_t ord = mod - (seq_t)1;          /* start with p-1 */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t prime = pf[i];
        /* try to divide `ord` by `prime` as many times as possible */
        while ((ord % (seq_t)prime) == (seq_t)0) {
            seq_t tmp = ord / (seq_t)prime;
            uint64_t exp = (uint64_t)tmp;
            seq_t val = powmod(base, exp, mod);
            if (val == (seq_t)1) {
                ord = tmp;
            } else {
                break;
            }
        }
    }
    return ord;
}

/* main sequence function */
seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* n-th prime, 1‑based */
    if (p == 2) return (seq_t)1;        /* special case */
    if (p == 5) return (seq_t)2;        /* special case */

    uint64_t mod_minus1 = (uint64_t)p - 1;
    uint64_t prime_factors[64];
    size_t pf_cnt = _factorize(mod_minus1, prime_factors, 64);

    /* order of 10 mod p */
    seq_t d = _order((seq_t)10, (seq_t)p, prime_factors, pf_cnt);

    /* number of distinct cycles */
    seq_t r = ((seq_t)p - (seq_t)1) / d;

    /* search for the least b with the required property */
    for (uint32_t b = 1; b < p; ++b) {
        seq_t ord_b = _order((seq_t)b, (seq_t)p, prime_factors, pf_cnt);
        seq_t g = gcd(ord_b, d);
        if ((ord_b / g) == r) {
            return (seq_t)b;
        }
    }

    /* fallback (should not occur) */
    return ((seq_t)p - (seq_t)1);
}
