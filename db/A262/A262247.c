/*
 * A262247  Number of squares formed from a square composed of p^2 unit
 *          squares where p is n-th prime.
 * https://oeis.org/A262247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (prime(n)+1) * (2*prime(n)+1) / 6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
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
    /* Fetch the n-th prime (p). */
    seq_t p = (seq_t)get_prime(n);

    /* Prepare the three factors. */
    seq_t a1 = p;
    seq_t b1 = p + 1;
    seq_t c1 = (seq_t)2 * p + 1;

    /* Divide by 2 (exactly one of a1 or b1 is even). */
    if (a1 % (seq_t)2 == 0)
        a1 /= (seq_t)2;
    else
        b1 /= (seq_t)2;  /* b1 is even in this case */

    /* Divide by 3 (at least one factor is divisible by 3). */
    if (a1 % (seq_t)3 == 0)
        a1 /= (seq_t)3;
    else if (b1 % (seq_t)3 == 0)
        b1 /= (seq_t)3;
    else
        c1 /= (seq_t)3;  /* c1 is divisible by 3 in this case */

    /* Multiply the reduced factors to get the result. */
    seq_t res = a1;
    res *= b1;
    res *= c1;
    return res;
}
