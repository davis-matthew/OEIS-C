/*
 * A392519  Numbers k such that sigma(k) = psi(k) + tau(k)^4 + omega(k).
 * https://oeis.org/A392519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1297 * p_n^2 where p_n is the n-th prime * (ignoring the
 *            prime 1297 itself). The sequence is therefore * 1297·p^2 for
 *            every prime p ≠ 1297. The first term (n=1) is * 1297·2^2 = 5188.
 *            For n=0 the function returns 0 (offset is 1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return 0;          /* offset is 1, so n≥1 in normal use */

    idx_t i = 1;          /* 1‑based index into the prime list */
    idx_t count = 0;      /* number of primes accepted so far */
    uint64_t p = 0;       /* prime value */

    while (count < n) {
        p = get_prime(i);   /* fetch i‑th prime (unsigned 64‑bit) */
        i++;
        if (p == 1297)       /* skip the forbidden prime */
            continue;
        count++;
    }

    seq_t ps = (seq_t)p;
    seq_t sq = ps * ps;
    return (seq_t)1297 * sq;
}
