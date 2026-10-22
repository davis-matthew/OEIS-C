/*
 * A327442  a(0) = 0; thereafter a(n) = a(n-1) + phi(n) if phi(n) > a(n-1),
 *          otherwise a(n) = a(n-1) - phi(n), where phi is the Euler
 *          phi-function A000010.
 * https://oeis.org/A327442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; thereafter a(n) = a(n-1) + phi(n) if phi(n) > a(n-1),
 *            otherwise a(n) = a(n-1) - phi(n)
 * Range:     u8 n <= 156, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A327442.  */

#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;      /* array of a(0..k-1) */
    static size_t  cap   = 0;        /* allocated capacity */
    static idx_t   len   = 0;        /* number of elements stored */

    /* initialise cache for a(0) if necessary */
    if (len == 0) {
        /* allocate for a(0) only */
        cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)0;
        len = 1;
    }

    /* extend cache if required */
    if (n + 1 > (idx_t)cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)(n + 1))
            new_cap = new_cap == 0 ? 1 : new_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort(); /* out‑of‑memory: terminate */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t phi_i = phi(i);                /* phi(i) as seq_t */
        seq_t prev  = cache[i - 1];
        if (phi_i > prev)
            cache[i] = prev + phi_i;
        else
            cache[i] = prev - phi_i;
    }
    len = n + 1;

    return cache[n];
}
