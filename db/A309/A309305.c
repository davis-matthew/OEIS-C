/*
 * A309305  Sums of two primes whose difference is a nonzero square.
 * https://oeis.org/A309305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A309305:
   a(n) = the nth integer that can be written as p + q,
   where p and q are primes and q - p = d^2 for some
   non‑zero integer d. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;      /* cached sequence values   */
static size_t terms_len = 0;     /* number of cached terms   */
static size_t terms_cap = 0;     /* allocated capacity       */
static seq_t next_candidate = (seq_t)5; /* next integer to test */

/* Ensure that the cache contains at least n terms. */
static void
ensure_terms(idx_t n)
{
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) abort();
    }

    while (terms_len < n) {
        bool found = false;

        /* Search for the next admissible sum. */
        for (;;) {
            /* Iterate over squares d² with d ≥ 1 and d² < next_candidate. */
            for (seq_t d = (seq_t)1;; ++d) {
                seq_t d2 = d * d;
                if (d2 >= next_candidate) break;

                /* next_candidate - d² must be even for integer p. */
                if (((next_candidate - d2) & (seq_t)1) == (seq_t)0) {
                    seq_t p = (next_candidate - d2) / (seq_t)2;
                    seq_t q = p + d2;
                    if (is_prime_big(p) && is_prime_big(q)) {
                        found = true;
                        break;
                    }
                }
            }

            if (found) {
                /* Grow the cache if necessary. */
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (tmp == NULL) abort();
                    terms = tmp;
                }
                terms[terms_len++] = next_candidate;
                next_candidate++;
                break;  /* finish this candidate */
            }
            next_candidate++;  /* try the next integer */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) abort();
    ensure_terms(n);
    return terms[n - (idx_t)1];
}
