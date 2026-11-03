/*
 * A249421  A(n,k) = exponent of the largest power of n-th prime which divides
 *          the product of the elements on row (k-1) of Pascal's triangle; a
 *          square array read by antidiagonals.
 * https://oeis.org/A249421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence: A249421
 *
 * a(n) = exponent of the n-th prime in the product of all entries on
 * the (k-1)-th row of Pascal's triangle, where (n,k) are read from the
 * square array A(n,k) by antidiagonals.  The function implements the
 * explicit computation described in the OEIS entry, using Legendre's
 * formula for factorial exponents.
 */

#include <stdlib.h>
#include <stdbool.h>
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
    /* --- Determine the antidiagonal position (i,k) for index n --- */
    idx_t s = 2;                                   /* current sum i+j */
    while ((s - 1) * s / 2 < n) ++s;               /* find minimal s */
    idx_t prev   = (s - 2) * (s - 1) / 2;          /* entries before this sum */
    idx_t offset = n - prev - 1;                   /* 0‑based index on the diagonal */
    idx_t i = offset + 1;                          /* row index (prime index)          */
    idx_t j = (s - 1) - offset;                    /* column index (k)                 */
    idx_t k = j - 1;                               /* row of Pascal’s triangle         */

    /* --- Find the n-th prime --- */
    uint32_t prime = get_prime(i);

    /* --- Compute exponent of the prime in the product of row k --- */
    /* v_p(k!) via Legendre's formula */
    seq_t vp_k_fact = 0;
    {
        idx_t pp = prime;
        while (pp <= k) {
            vp_k_fact += k / pp;
            /* Safe multiplication: pp <= k <= 140, so no overflow */
            pp *= prime;
        }
    }

    /* Sum_{t=0}^{k} v_p(t!) using cumulative counting */
    seq_t sum_vpfact = 0;
    seq_t vp_fact    = 0;
    for (idx_t t = 1; t <= k; ++t) {
        idx_t temp = t;
        while (temp % prime == 0) {
            ++vp_fact;
            temp /= prime;
        }
        sum_vpfact += vp_fact;     /* add v_p(t!) to the running sum */
    }

    /* Final exponent: (k+1)*v_p(k!) - 2*sum_{t=0}^{k} v_p(t!) */
    seq_t exp = (seq_t)(k + 1) * vp_k_fact - (seq_t)2 * sum_vpfact;
    return exp;
}
