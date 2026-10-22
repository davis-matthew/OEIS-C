/*
 * A355984  Primes whose reversal is a multiple of 8.
 * https://oeis.org/A355984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime whose decimal reversal is a multiple of 8
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by linear search with memoisation.          */

#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms       = NULL;
    static size_t terms_len   = 0;
    static size_t terms_cap   = 0;

    /* Next integer to test for primality (64‑bit is sufficient for the
       intended limits; cast to seq_t for the primality test). */
    static uint64_t next_candidate = 2;

    if (n < (idx_t)1) return (seq_t)0;

    /* Grow the cache until it contains at least n terms. */
    while (terms_len < (size_t)n) {
        /* Find the next prime ≥ next_candidate. */
        while (!is_prime_big((seq_t)next_candidate)) {
            ++next_candidate;
        }

        /* Apply the defining condition. */
        seq_t rev = reverse_decimal((seq_t)next_candidate);
        if ((rev % (seq_t)8) == (seq_t)0) {
            /* Allocate space for a new entry if needed. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* Out of memory. */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }

        /* Move to the next integer for the next iteration. */
        ++next_candidate;
    }

    return terms[n - 1];   /* a(1) is stored at index 0. */
}
