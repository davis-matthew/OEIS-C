/*
 * A283806  Odd numbers which are uniquely decomposable into the sum of a
 *          prime and a power of two.
 * https://oeis.org/A283806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283806: odd numbers uniquely decomposable as prime + power of two.
   Search‑based implementation that caches discovered terms. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of discovered terms */
    static seq_t *terms = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    /* first valid index is 1 */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* initialise cache */
    if (!terms) {
        cap   = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms)
            return (seq_t)0;   /* out of memory -> return 0 */
    }

    /* extend cache until it contains at least n terms */
    while (len < (size_t)n) {
        /* next odd candidate */
        seq_t cand = len == 0 ? (seq_t)3 : terms[len - 1] + (seq_t)2;

        /* find the next valid term */
        while (1) {
            size_t prime_cnt = 0;
            seq_t   pow = (seq_t)1;

            /* iterate over powers of two <= cand */
            for (; pow <= cand; ) {
                seq_t diff = cand - pow;
                if (diff >= (seq_t)2 && is_prime_big(diff))
                    ++prime_cnt;
                if (prime_cnt > 1)
                    break; /* more than one decomposition */

                /* avoid overflow on the next multiplication */
                if (pow > cand / (seq_t)2)
                    break;
                pow *= (seq_t)2;
            }

            if (prime_cnt == 1) {
                /* store the term, grow array if necessary */
                if (len == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp)
                        return (seq_t)0;   /* realloc failed */
                    terms = tmp;
                    cap   = new_cap;
                }
                terms[len++] = cand;
                break; /* proceed to next candidate */
            }

            cand += (seq_t)2;   /* try the next odd number */
        }
    }

    return terms[n - 1];
}
