/*
 * A379851  Numbers k such that phi(k) does not divide k. Complement of
 *          A007694.
 * https://oeis.org/A379851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379851: numbers k such that φ(k) does NOT divide k
   (complement of A007694).  Implemented by incremental search. */

#include <stdlib.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence is 1‑based, but gracefully handle 0 */
        return (seq_t)0;

    /* Persistent cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static idx_t next   = 3;     /* search starts at 3, the first valid term */

    /* Grow the cache until it contains at least n terms */
    while (len < n)
    {
        seq_t cand   = (seq_t)next;          /* value for φ and storage */
        seq_t phi_val = calc_phi(cand);

        if (cand % phi_val != (seq_t)0)      /* φ(cand) does not divide cand */
        {
            if (len == cap)                 /* resize if necessary */
            {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;             /* store the term */
        }
        ++next;                              /* examine next integer */
    }

    return cache[(size_t)(n - 1)];
}
