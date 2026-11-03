/*
 * A366923  a(n) = A110936(n)/6 for n >= 3.
 * https://oeis.org/A366923
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A110936(n)/6 where * A110936(n) = product of all primes q
 *            < prime[n] such that (prime[n]-1) % (q-1) == 0
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 41, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 60 (2026-11-01)
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
    if (n < 3) return 0;

    /* prime of index n */
    uint32_t p = get_prime(n);
    uint64_t p_minus1 = (uint64_t)p - 1;

    seq_t prod = 1;

    /* iterate over all earlier primes */
    for (idx_t i = 1; i < n; ++i) {
        uint32_t q = get_prime(i);
        uint64_t q_minus1 = (uint64_t)q - 1;
        if (q_minus1 != 0 && p_minus1 % q_minus1 == 0)
            prod *= (seq_t)q;
    }

    /* divide by 6 (product always divisible by 6) */
    prod /= 6;
    return prod;
}
