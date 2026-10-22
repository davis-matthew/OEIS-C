/*
 * A375361  Odd numbers with at least two prime divisors of the form 4*k + 1
 *          counted with multiplicity.
 * https://oeis.org/A375361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence is odd integers having at least two prime factors
 *            congruent to 1 (mod 4) (counted with multiplicity). The function
 *            builds the list lazily and returns the n‑th term (1‑indexed)
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    static bool inited = false;
    static uint64_t *primes = NULL;          /* primes for trial division */
    static size_t primes_cnt = 0;
    static seq_t *terms = NULL;             /* cached sequence terms */
    static size_t terms_cap = 0;
    static size_t terms_filled = 0;
    static idx_t next_candidate = 1;        /* next odd integer to test */

    if (!inited) {
        /* A bound sufficient for n up to about 10 000. */
        const idx_t PRIME_LIMIT = 1000000;
        build_primes_upto(PRIME_LIMIT, (uint64_t **)&primes, &primes_cnt);

        terms_cap = 1024;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        inited = true;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_filled < n) {
        idx_t x = next_candidate;   /* odd integer being tested */
        idx_t temp = x;
        idx_t count = 0;            /* number of 1(mod 4) prime factors
                                       (counted with multiplicity) */

        /* Trial division using the precomputed primes. */
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p * p > temp) break;
            if (temp % p == 0) {
                idx_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if ((p % 4) == 1) count += exp;
            }
        }

        /* Remaining prime factor, if any. */
        if (temp > 1 && (temp % 4) == 1) {
            ++count;
        }

        if (count >= 2) {
            if (terms_filled == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_filled++] = (seq_t)x;
        }

        next_candidate += 2; /* proceed to the next odd integer */
    }

    return terms[n - 1];
}
