/*
 * A273801  Numbers n for which n = (x - phi(x)) * (y - phi(y)), where n = x +
 *          y and x - phi(x) is the Euler cototient function of x.
 * https://oeis.org/A273801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (prime(n+1) + 1)
 * Range:     u8 n <= 17, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 70 terms through n = 194 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following header provides the get_prime(k) utility that returns
 * the k‑th prime (1‑based). It is part of the utilities bundle.
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
    /* Use the (n+1)‑th prime in the OEIS formula. */
    idx_t k = n + 1;                     /* 1‑based index of the desired prime */
    seq_t p = (seq_t)get_prime(k);       /* convert prime to seq_t */
    seq_t res = ((seq_t)4) * (p + 1);    /* apply the formula */
    return res;
}
