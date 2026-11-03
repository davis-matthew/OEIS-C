/*
 * A293514  a(n) = Product_{d|n, d>1} prime(A286561(n,d)), where A286561(n,d)
 *          gives the highest exponent of d dividing n.
 * https://oeis.org/A293514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime( A286561(n,d) )
 * Range:     u8 n <= 23, u16 n <= 119, u32 n <= 719, u64 n <= 7559,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 9909 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A286561(n,d) is the largest e such that d^e divides n */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

static void _rec_divisors(size_t pos,
                          const uint64_t *pr,
                          const uint64_t *exp,
                          size_t m,
                          uint64_t *sel,
                          seq_t *prod)
{
    if (pos == m) {
        /* skip divisor 1 */
        int allzero = 1;
        for (size_t k = 0; k < m; ++k) {
            if (sel[k]) { allzero = 0; break; }
        }
        if (allzero) return;

        /* compute valuation of this divisor in n */
        uint64_t e = UINT64_MAX;
        for (size_t k = 0; k < m; ++k) {
            if (sel[k]) {
                uint64_t cand = exp[k] / sel[k];
                if (cand < e) e = cand;
            }
        }

        /* multiply the corresponding prime */
        seq_t p = (seq_t)get_prime(e);
        *prod *= p;
        return;
    }

    for (uint64_t f = 0; f <= exp[pos]; ++f) {
        sel[pos] = f;
        _rec_divisors(pos + 1, pr, exp, m, sel, prod);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* maximum number of distinct prime factors for 64‑bit numbers is 16 */
    uint64_t pr[16];
    uint64_t exp[16];
    size_t  m = 0;

    factor_u64(n, pr, exp, &m);

    uint64_t sel[16] = {0};
    seq_t prod = (seq_t)1;

    _rec_divisors(0, pr, exp, m, sel, &prod);

    return prod;
}
