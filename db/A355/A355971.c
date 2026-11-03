/*
 * A355971  Numerator of the cross-ratio of the four primes p_n, p_{n+1},
 *          p_{n+2}, p_{n+3}, where p_n = prime(n).
 * https://oeis.org/A355971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ((pₙ-pₙ₊₂)/(pₙ-pₙ₊₃)) *
 *            ((pₙ₊₁-pₙ₊₃)/(pₙ₊₁-pₙ₊₂)) where pₙ is the n‑th prime and p₁ = 2.
 *            The function computes the numerator of the fully reduced
 *            cross‑ratio
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 109 terms through n = 939 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>

/* Euclidean algorithm for unsigned 64‑bit integers */
static inline unsigned long long gcd_ull(unsigned long long a,
                                         unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Indices of the four consecutive primes */
    idx_t n0 = n;
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;
    idx_t n3 = n + 3;

    /* Retrieve the primes (uint32_t) */
    uint32_t p0 = get_prime(n0);
    uint32_t p1 = get_prime(n1);
    uint32_t p2 = get_prime(n2);
    uint32_t p3 = get_prime(n3);

    /* Compute differences as 64‑bit values to avoid overflow */
    unsigned long long d_a = (unsigned long long)p0 - (unsigned long long)p2; /* pₙ - pₙ₊₂ */
    unsigned long long d_b = (unsigned long long)p0 - (unsigned long long)p3; /* pₙ - pₙ₊₃ */
    unsigned long long d_c = (unsigned long long)p1 - (unsigned long long)p3; /* pₙ₊₁ - pₙ₊₃ */
    unsigned long long d_d = (unsigned long long)p1 - (unsigned long long)p2; /* pₙ₊₁ - pₙ₊₂ */

    /* Numerator and denominator before simplification */
    unsigned long long num = d_a * d_c;
    unsigned long long den = d_b * d_d;

    /* Reduce by gcd and return the simplified numerator */
    unsigned long long g   = gcd_ull(num, den);
    unsigned long long n_reduced = num / g;

    return (seq_t)n_reduced;
}
