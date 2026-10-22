/*
 * A389065  Numbers that are neither prime powers nor powerful.
 * https://oeis.org/A389065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that are composite, not a prime power, and not powerful. */
/* Sequence: A389065 – Numbers that are neither prime powers nor powerful. */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t types are defined by the build system. */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *terms = NULL;           /* array of already computed terms          */
    static idx_t terms_cap = 0;           /* current capacity of the array           */
    static idx_t terms_count = 0;         /* number of terms stored so far           */
    static uint64_t cur = 2;              /* next integer to test                    */

    /* Expand the array if needed. */
    if (terms_count < n) {
        /* Ensure the terms array is large enough for all required terms. */
        if (terms == NULL) {
            terms_cap = 256;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        } else {
            while (terms_count < n && terms_cap < (terms_count + 128))
                terms_cap <<= 1;
            if (terms_cap > terms_count)
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }

        /* Find additional terms until we have at least n of them. */
        while (terms_count < n) {
            uint64_t val = cur;
            cur++;

            /* Factorise val and examine exponents. */
            uint64_t tmp = val;
            int    numPrimes = 0;
            bool   hasExpOne = false;

            /* handle factor 2 */
            if ((tmp & 1ULL) == 0) {
                int c = 0;
                while ((tmp & 1ULL) == 0) {
                    tmp >>= 1;
                    ++c;
                }
                ++numPrimes;
                if (c == 1) hasExpOne = true;
            }

            /* handle odd factors */
            for (uint64_t p = 3; p <= tmp / p; p += 2) {
                if (tmp % p == 0) {
                    int c = 0;
                    while (tmp % p == 0) {
                        tmp /= p;
                        ++c;
                    }
                    ++numPrimes;
                    if (c == 1) hasExpOne = true;
                }
            }

            /* if any prime factor remains, it has exponent 1. */
            if (tmp > 1) {
                ++numPrimes;
                hasExpOne = true;
            }

            /* Accept if composite with at least two distinct primes and at least one exponent = 1. */
            if (numPrimes >= 2 && hasExpOne) {
                if (terms_count >= terms_cap) {
                    terms_cap <<= 1;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_count++] = (seq_t)val;
            }
        }
    }

    /* All terms are stored in 0‑based index, but n is 1‑based. */
    return terms[n - 1];
}
