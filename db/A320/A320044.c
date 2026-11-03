/*
 * A320044  Number of positive integers k < prime(n)/2 with {k^3/prime(n)} >
 *          1/2, where {x} = x - floor(x) is the fractional part of a real
 *          number x.
 * https://oeis.org/A320044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9956 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A320044:  a(n) = number of k < prime(n)/2
   for which the fractional part of k³ / prime(n) exceeds 1/2. */

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

/* Count the desired k for a single prime p.  p is assumed to be
   a 32‑bit unsigned integer from get_prime. */
static seq_t count_for_prime(uint32_t p)
{
    if (p <= 2u)            /* no k satisfies k < p/2 when p <= 2 */
        return 0;

    seq_t half = p / 2u;                      /* floor(p/2) as seq_t   */
    idx_t limit = (p - 1u) / 2u;              /* largest k to test    */

    seq_t count = 0;
    for (idx_t k = 1; k <= limit; ++k)
    {
        /* compute k³ mod p using 64‑bit arithmetic */
        uint64_t kmul = (uint64_t)k * k % p;  /* k² mod p */
        kmul = kmul * k % p;                 /* k³ mod p */

        if ((seq_t)kmul > half)              /* fractional part > 1/2? */
            ++count;
    }
    return count;
}

/* Public entry point for the sequence.  n is 1‑based (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;                          /* undefined for n=0, stay safe */

    uint32_t p = get_prime(n);               /* prime(n)   */
    return count_for_prime(p);
}
