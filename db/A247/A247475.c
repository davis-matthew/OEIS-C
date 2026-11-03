/*
 * A247475  Numbers formed by concatenating two successive primes and their
 *          product.
 * https://oeis.org/A247475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate( p_n, p_{n+1}, p_n * p_{n+1} ) where p_n is
 *            the n-th prime. Offset 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 3401,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 39 terms through n = 8335 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for the utilities used */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term.  For n < 1 the sequence is undefined. */
    if (n < 1) return (seq_t)0;

    /* Get the successive primes.  get_prime returns a 32‑bit value. */
    uint32_t p1 = get_prime(n);       /* p_n   */
    uint32_t p2 = get_prime(n + 1);   /* p_{n+1} */

    /* Convert to seq_t for arithmetic. */
    seq_t sp1 = (seq_t)p1;
    seq_t sp2 = (seq_t)p2;

    /* Product of the two primes. */
    seq_t prod = sp1 * sp2;

    /* Find how many decimal digits the product has. */
    int d_prod = 1;
    seq_t p10   = pow10(1);  /* 10^1 in seq_t form */
    while (prod >= p10)
    {
        ++d_prod;
        p10 = p10 * (seq_t)10;  /* shift left by one decimal place */
    }

    /* Concatenate p1 and p2 first. */
    seq_t part12 = concat_parts(p1, p2);

    /* Finally append the product: part12 * 10^d_prod + prod. */
    seq_t result = part12 * pow10(d_prod) + prod;

    return result;
}
