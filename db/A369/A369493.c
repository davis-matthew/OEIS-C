/*
 * A369493  Table read by rows: row n is the unique primitive Pythagorean
 *          triple (a,b,c) such that (a-b+c)/2 = prime(n) and the short leg
 *          "a" is odd.
 * https://oeis.org/A369493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(3k+1) = 2*prime(k) - 1, a(3k+2) = 2*prime(k)^2 - 2*prime(k),
 *            a(3k+3) = 2*prime(k)^2 - 2*prime(k) + 1
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    /* n starts at 1; compute the row index k */
    idx_t k = (n - 1) / 3 + 1;
    /* position within the triple */
    idx_t pos = (n - 1) % 3;

    /* obtain the k-th prime */
    uint32_t p_u32 = get_prime(k);
    seq_t p = (seq_t)p_u32;

    /* compute the three triple components */
    seq_t a_val = (seq_t)2 * p - 1;
    seq_t b_val = (seq_t)2 * p * p - (seq_t)2 * p;   /* 2*p*(p-1) */
    seq_t c_val = b_val + 1;

    /* return the requested term */
    switch (pos) {
        case 0: return a_val;
        case 1: return b_val;
        case 2: return c_val;
        default: return 0; /* unreachable */
    }
}
