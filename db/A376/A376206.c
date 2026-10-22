/*
 * A376206  Prime indices of triangular numbers k such that either k + 1 or k
 *          - 2 is prime.
 * https://oeis.org/A376206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A376206 - Prime indices of triangular numbers k such that either k + 1 or k – 2 is prime.
   a(n) is the n‑th prime p (1‑based) with the property that T(p)=p(p+1)/2 satisfies
   is_prime(T(p)+1) || is_prime(T(p)-2). */

/* Helper: prime test for idx_t values. */
#include "../utils/is_prime.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;   /* array of sequence values */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of valid elements */
    static idx_t  next  = 2;      /* candidate prime to test */

    /* Generate terms until at least n are found */
    while (len < n) {
        /* Skip composite candidates */
        while (!is_prime(next))
            ++next;

        /* Compute the triangular number for the prime next */
        idx_t tri = next * (next + 1) / 2;
        /* Check the two required conditions */
        if (is_prime(tri + 1) || is_prime(tri - 2)) {
            /* Ensure capacity */
            if (len == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = (seq_t)next;    /* store the prime itself */
        }
        ++next;                          /* advance candidate */
    }

    return cache[n - 1];
}
