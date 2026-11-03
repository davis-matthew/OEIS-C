/*
 * A326952  a(n) = A001222(A028905(n)).
 * https://oeis.org/A326952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bigOmega( sorted_digits( prime(n) ) ) where prime(n) is
 *            the nth prime number (OEIS A000040), sorted_digits arranges the
 *            decimal digits of the prime in nondecreasing order and
 *            interprets them as a decimal integer. bigOmega is the total
 *            number of prime factors counting multiplicity (OEIS A00...
 * Range:     u8 n <= 54, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Validated: standard, pass, 116 terms through n = 8821 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"

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
    /* the sequence is offset 1; values for n<1 are unused */
    if (n == 0) return 0;

    /* step 1: obtain the n-th prime (fits in uint32_t for n<=10000) */
    uint32_t p = get_prime(n);          /* get_prime is provided */

    /* step 2: collect digit frequencies of p */
    unsigned int freq[10] = {0};
    uint32_t tmp = p;
    if (tmp == 0) freq[0] = 1;          /* only for safety */
    while (tmp > 0) {
        freq[tmp % 10]++;
        tmp /= 10;
    }

    /* step 3: build the sorted‑digit number */
    seq_t m = 0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned int k = 0; k < freq[d]; ++k) {
            m = m * 10 + d;
        }
    }

    /* step 4: count prime factor multiplicities of m */
    seq_t result = bigomega_seq(m);   /* count_prime_factors is provided */

    return result;
}
