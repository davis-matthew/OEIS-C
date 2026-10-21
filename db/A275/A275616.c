/*
 * A275616  Numbers k such that k and omega(k) are relatively prime, where
 *          omega(n) (A001221) is the number of distinct prime divisors of n.
 * https://oeis.org/A275616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that gcd(k, omega(k)) = 1, computed by
 *            incrementally sieving the number of distinct prime divisors
 *            omega(k)
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Static state to keep the cache across calls */
    static idx_t current_limit = 0;   /* last sieve limit */
    static idx_t terms_count   = 0;   /* number of terms cached */
    static seq_t *terms        = NULL;/* cached sequence values */

    /* Ensure n ≥ 1; the sequence is offset 1. */
    if (n == 0) return (seq_t)0;

    /* Extend the cache until we have at least n terms. */
    while (terms_count < n) {
        idx_t new_limit = current_limit ? current_limit * 2 : 100;
        if (new_limit < n * 3) new_limit = n * 3;  /* guarantee enough terms */
        if (new_limit < 100) new_limit = 100;

        /* Allocate and initialise omega[n] array (distinct prime counts). */
        uint8_t *omega = (uint8_t *)malloc((new_limit + 1) * sizeof(uint8_t));
        if (!omega) abort();                     /* memory allocation failed */
        for (idx_t i = 0; i <= new_limit; ++i) omega[i] = 0;

        /* Sieve to compute omega: for each prime, increment its multiples. */
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (omega[p] == 0) {               /* p is prime */
                for (idx_t m = p; m <= new_limit; m += p)
                    ++omega[m];
            }
        }

        /* Re‑allocate the terms array for the new sequence. */
        seq_t *new_terms = (seq_t *)malloc(new_limit * sizeof(seq_t));
        if (!new_terms) abort();                /* memory allocation failed */

        /* Enumerate all integers up to new_limit and collect the good ones. */
        idx_t count = 0;
        for (idx_t k = 1; k <= new_limit; ++k) {
            seq_t x = (seq_t)k;
            seq_t w = (seq_t)omega[k];
            if (gcd(x, w) == (seq_t)1) {
                new_terms[count++] = x;
            }
        }

        /* Replace old cache with the new one. */
        free(terms);
        terms = new_terms;
        terms_count = count;
        current_limit = new_limit;

        free(omega);
    }

    /* Return the requested term (1‑based index). */
    return terms[n - 1];
}
