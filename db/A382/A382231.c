/*
 * A382231  Octagonal numbers that are the product of three distinct primes.
 * https://oeis.org/A382231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th octagonal number that is the product of three
 *            distinct primes
 * Range:     u8 none, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The helper that factorises a 64‑bit integer. */
#include "../utils/factor_u64.h"

static seq_t *terms   = NULL;   /* dynamically grown array of terms */
static size_t terms_len = 0;    /* number of terms stored */
static size_t terms_cap = 0;    /* current capacity of the array */
static uint64_t search_k = 1;   /* next octagonal index to test */

static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Ensure that we have computed at least n terms. */
    while (terms_len < (size_t)n) {
        uint64_t k = search_k++;
        seq_t kk = (seq_t)k;
        seq_t oct = kk * ((kk * 3) - 2);   /* k*(3k-2)  -- octagonal number */

        /* The octagonal number fits in 64 bits for the range of n we need. */
        uint64_t val = (uint64_t)oct;

        /* Factorise the 64‑bit value. */
        uint64_t primes[128];
        uint64_t exps[128];
        size_t cnt = 0;
        factor_u64(val, primes, exps, &cnt);

        /* It is a product of three distinct primes iff there are exactly
           three distinct factors and each has exponent 1. */
        if (cnt == 3 &&
            exps[0] == 1 && exps[1] == 1 && exps[2] == 1) {
            append_term(oct);
        }
    }

    return terms[n - 1];
}
