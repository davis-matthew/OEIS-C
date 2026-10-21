/*
 * A276049  Isolated deficient numbers that are prime.
 * https://oeis.org/A276049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1798, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Brute‑force search for primes whose adjacent
 * composite numbers are not deficient (i.e., sigma(x) ≥ 2·x). */

#include <stddef.h>
#include <stdbool.h>

/* Utility prototypes supplied by the build system */
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/ensure_capacity.h"

/* Static cache for the sequence */
static seq_t *terms      = NULL;   /* array of found terms          */
static size_t terms_cap  = 0;      /* allocated capacity of array   */
static size_t terms_len  = 0;      /* number of terms stored        */
static idx_t  next_cand  = 3;      /* next odd number to test       */

/* Return the n‑th term (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1         */

    /* Grow the cache until it holds at least n terms. */
    while (terms_len < (size_t)n) {
        /* Find the next prime candidate (only odd numbers). */
        while (!is_prime(next_cand))
            next_cand += 2;

        idx_t p = next_cand;        /* current prime candidate */

        /* Neighboring composite numbers of a prime p > 2 are even. */
        seq_t left  = p - 1;
        seq_t right = p + 1;

        seq_t sigma_left  = sigma_of((idx_t)left);
        seq_t sigma_right = sigma_of((idx_t)right);

        bool left_deficient  = sigma_left  < (2 * left);
        bool right_deficient = sigma_right < (2 * right);

        /* Store p if neither neighbor is deficient. */
        if (!left_deficient && !right_deficient) {
            ensure_capacity(terms_len + 1, &terms_cap, (void **)&terms, sizeof(seq_t));
            terms[terms_len++] = p;
        }

        /* Advance to the next odd candidate. */
        next_cand += 2;
    }

    return terms[n - 1];
}
