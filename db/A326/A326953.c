/*
 * A326953  a(n) = A001222(A028906(n)).
 * https://oeis.org/A326953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Ω( reverse_sorted_digits( prime(n) ) ) where
 *            reverse_sorted_digits gets the digits of the nth prime, sorts
 *            them descending and concatenates them into a number m; Ω(m) is
 *            the total number of prime factors (with multiplicity) of m
 * Range:     u8 n <= 9592, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Validated: standard, pass, 116 terms through n = 9431 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bigomega.h"

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
    /* Prime number of index n */
    uint32_t p = get_prime(n);

    /* Frequency of each decimal digit in p */
    int freq[10] = {0};
    uint32_t t = p;
    if (t == 0) {          /* safety, never happens for primes > 1 */
        freq[0] = 1;
    } else {
        while (t > 0) {
            int d = t % 10;
            freq[d]++;
            t /= 10;
        }
    }

    /* Build the number with digits sorted in descending order */
    uint64_t m = 0;
    for (int d = 9; d >= 0; --d)
        for (int k = 0; k < freq[d]; ++k)
            m = m * 10 + (uint64_t)d;

    /* Return the number of prime factors of m, counted with multiplicity */
    return bigomega((idx_t)m);
}
