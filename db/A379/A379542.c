/*
 * A379542  Second term of the n-th differences of the prime numbers.
 * https://oeis.org/A379542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * binomial(n,k) * prime(k+2)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 29, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (prime(2)=3, prime(3)=5, …, where prime(k+2) is the (k+1)-st odd prime) */

#include <stdint.h>                     /* for uint32_t */
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
    seq_t sum   = 0;                /* running total */
    seq_t binom = 1;                /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* prime(k+2): skip the first prime (2) to get the odd primes */
        seq_t prime = (seq_t)get_prime(k + 2);

        /* term = C(n,k) * prime(k+2) */
        seq_t term = binom * prime;

        /* add or subtract depending on parity of (n-k) */
        if (((n - k) & 1) == 0)
            sum += term;
        else
            sum -= term;

        /* update binomial coefficient for next k, unless we are at the end */
        if (k < n)
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return sum;
}
