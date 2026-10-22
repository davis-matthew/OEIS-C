/*
 * A342591  Numbers k such that A340740(k) <= k.
 * https://oeis.org/A342591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that
   Sum_{1 ≤ j ≤ ⌊k/2⌋ , gcd(j,k)=1} (k mod j) ≤ k.  */

/*  seq_t and idx_t are defined by the build system.
    This function memoises the values of the sequence.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Persistent storage for the terms found so far. */
    static seq_t *terms   = NULL;
    static size_t terms_len  = 0;
    static size_t terms_cap  = 0;
    static idx_t last_k_checked = 0;

    /* Helper to grow the array when needed. */
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? (terms_cap << 1) : 8;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: return 0 */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* Generate terms until the desired index is reached. */
    while (terms_len < n) {
        idx_t upper = (idx_t)((last_k_checked + 1) / 2);
        seq_t k = (seq_t)(last_k_checked + 1);
        seq_t sum = 0;

        last_k_checked = (idx_t)k;  /* update for next iteration */

        for (idx_t j = 1; j <= upper; ++j) {
            if (gcd((seq_t)j, k) == (seq_t)1) {
                sum += k % (seq_t)j;
            }
        }

        if (sum <= k) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? (terms_cap << 1) : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
    }

    return terms[n - 1];
}
