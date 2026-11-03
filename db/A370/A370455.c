/*
 * A370455  a(n) = greatest m such that 2^m divides prime(n+1)*prime(n+2) -
 *          prime(n)*prime(n+3).
 * https://oeis.org/A370455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2-adic valuation of (prime(n+1)*prime(n+2) -
 *            prime(n)*prime(n+3))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
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
    /* primes required: p_n, p_{n+1}, p_{n+2}, p_{n+3} */
    seq_t p1 = (seq_t) get_prime(n);
    seq_t p2 = (seq_t) get_prime(n + 1);
    seq_t p3 = (seq_t) get_prime(n + 2);
    seq_t p4 = (seq_t) get_prime(n + 3);

    seq_t val = p2 * p3 - p1 * p4;
    /* take absolute value (the 2-adic valuation is the same for -val) */
    if (val < 0)
        val = -val;

    idx_t m = 0;
    /* count trailing zeros of val */
    while ((val & 1) == 0)
    {
        val >>= 1;
        m++;
    }

    return (seq_t) m;
}
