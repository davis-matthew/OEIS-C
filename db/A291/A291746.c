/*
 * A291746  Nonprimes of the form 5*k + 1.
 * https://oeis.org/A291746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th nonprime of the form 5*k+1 (k ≥ 0). The sequence
 *            starts with a(1)=1, a(2)=6, a(3)=16, …
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute the n‑th term of OEIS A291746 */
seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache    = NULL;   /* array of terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms stored */

    static idx_t next_k = 0;         /* next k to examine (k starts at 0) */

    if (n == 0)            /* undefined: return 0 for safety */
        return (seq_t)0;

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* Grow the buffer if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);      /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Current candidate: 5*k + 1 */
        seq_t candidate = (seq_t)next_k * (seq_t)5 + (seq_t)1;
        /* 1 is considered nonprime and is the first term */
        if (next_k == 0 || !is_prime((idx_t)candidate)) {
            cache[cache_len++] = candidate;
        }

        next_k++;        /* move to the next k for the next iteration */
    }

    return cache[n - 1];
}
