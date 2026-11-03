/*
 * A362243  a(n) = number of isomorphism classes of elliptic curves over the
 *          finite field of order prime(n).
 * https://oeis.org/A362243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=8, a(n)=2*prime(n)+C for n>2, * where C=6 if
 *            prime(n)%12==1, * C=2 if prime(n)%12==5, * C=4 if
 *            prime(n)%12==7, * C=0 if prime(n)%12==11
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9060 (2026-11-01)
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
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;

    /* get the nth prime (1‑based) */
    uint32_t p = get_prime(n);

    seq_t val = (seq_t)p * (seq_t)2;
    uint32_t mod = p % 12u;
    seq_t C;
    switch (mod) {
        case 1: C = (seq_t)6; break;
        case 5: C = (seq_t)2; break;
        case 7: C = (seq_t)4; break;
        case 11: C = (seq_t)0; break;
        default: C = (seq_t)0; /* should not occur for primes >3 */
    }

    return val + C;
}
