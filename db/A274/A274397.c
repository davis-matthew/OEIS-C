/*
 * A274397  Positive integers m such that sigma(m) is divisible by 5.
 * https://oeis.org/A274397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that sigma(m) is divisible by 5. * The function
 *            builds the list of terms on demand and memoizes the * results.
 *            It keeps a static array of the already found terms and *
 *            continues scanning natural numbers until the requested ordinal *
 *            has been reached
 * Range:     u8 n <= 20, u16 n <= 6706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Static state keeping the computed terms. */
    static seq_t *terms = NULL;      /* array of found terms             */
    static size_t terms_len = 0;     /* number of stored terms           */
    static size_t terms_cap = 0;     /* allocated capacity (bytes/elem)  */
    static idx_t next_candidate = 1; /* next integer to test for sigma   */

    /* Expand the cache until it contains at least n terms. */
    while (terms_len < (size_t)n) {
        /* Check the next number. */
        seq_t sigma = sigma_of(next_candidate);

        /* Store the term if sigma is a multiple of 5. */
        if ((sigma % 5) == 0) {
            /* Grow the array if necessary. */
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();                /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_candidate;
        }
        ++next_candidate;                    /* advance to the next m   */
    }

    /* Return the requested term (1‑based index). */
    return terms[n - 1];
}
