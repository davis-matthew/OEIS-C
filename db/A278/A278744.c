/*
 * A278744  Number of steps (modular reductions) in calculating the GCD of
 *          n-th consecutive primes p(n) and p(n+1) by the Euclidean
 *          algorithm.
 * https://oeis.org/A278744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of modular reductions in Euclid’s algorithm used
 *            to compute the GCD of the consecutive primes p(n) and p(n+1)
 * Range:     u8 n <= 53, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, callers normally start at 1 */

    /* Retrieve the two consecutive primes: p(n) and p(n+1).  The function
       returns a 32‑bit unsigned integer; we cast it to seq_t for the
       arithmetic that follows. */
    seq_t b_val = (seq_t)get_prime(n);       /* p(n)   */
    seq_t a_val = (seq_t)get_prime(n + 1);   /* p(n+1) */

    seq_t steps = 0;
    seq_t r;

    /* Perform the Euclidean algorithm, counting modular reductions
       until the remainder becomes 1 (the GCD of two distinct primes). */
    while (true) {
        r = a_val % b_val;
        ++steps;
        if (r == (seq_t)1)
            break;
        a_val = b_val;
        b_val = r;
    }

    return steps;
}
