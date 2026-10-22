/*
 * A339084  Smaller term p1 of the first of two consecutive cousin prime pairs
 *          (p1,p1+4) and (p2,p2+4) such that the distance (p2-p1) is a
 *          square.
 * https://oeis.org/A339084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A339084 – smaller term of the first of two consecutive cousin prime pairs
 *  (p1,p1+4) and (p2,p2+4) where the distance p2-p1 is a perfect square.
 *
 *  The implementation scans primes in ascending order, keeps track of the
 *  most recent cousin prime pair (first element of the pair), and whenever
 *  the distance to the next cousin pair is a square, appends the previous
 *  pair’s smaller prime to the sequence.  All results are cached so that
 *  calls with arbitrary index order are fast.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/floor_sqrt.h"

/* static cache of discovered sequence terms */
static seq_t *terms     = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* state variables used to generate cousin prime pairs */
static uint64_t p_curr         = 3ULL;   /* current prime being examined */
static uint64_t prev_candidate = 0ULL;   /* previous cousin-pair starting prime */

static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 16;
        seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms     = new_ptr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  External function requested by the exercise                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined, return 0 */

    /* generate terms until we have n of them */
    while (terms_len < (size_t)n) {
        /* p_curr is a prime; check if p_curr+4 is also prime */
        if (is_prime_small(p_curr + 4ULL)) {
            /* we have a cousin‑prime pair starting at p_curr */
            if (prev_candidate != 0ULL) {
                uint64_t dist = p_curr - prev_candidate;
                seq_t sqrt_d = floor_sqrt((seq_t)dist);
                if (sqrt_d * sqrt_d == (seq_t)dist) {
                    /* square distance – store the previous pair's smaller prime */
                    append_term((seq_t)prev_candidate);
                }
            }
            /* remember this pair for the next comparison */
            prev_candidate = p_curr;
        }

        /* advance to the next prime */
        p_curr = next_prime(p_curr);
    }

    return terms[(size_t)(n - 1)];
}
