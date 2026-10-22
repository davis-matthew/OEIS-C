/*
 * A359434  Numbers k >= 1 such that k * phi(k) / (k + phi(k)) is an integer,
 *          where phi(k) = A000010(k).
 * https://oeis.org/A359434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A359434: a(n) = the n‑th integer k ≥12 such that k*phi(k)/(k+phi(k)) is integer.
   The code generates terms on demand by testing successive integers and
   storing qualifying values in a static dynamic array. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"   /* declares: seq_t phi(idx_t m); */

typedef unsigned __int128 u128;

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cnt = 0;      /* number of terms stored             */
    static size_t cap = 0;      /* allocated capacity in elements      */
    static idx_t  start = 12;   /* next integer to test (≥12)         */

    if (n == 0) return 0; /* sequence is 1‑based */

    /* grow the cache until it contains at least n terms */
    while (cnt < (size_t)n) {
        seq_t k     = (seq_t)start;        /* current integer */
        seq_t phi_k = phi(start);          /* φ(k) */

        /* Use 128‑bit arithmetic to avoid overflow for 64‑bit seq_t. */
        u128 num = (u128)k * (u128)phi_k;
        u128 den = (u128)k + (u128)phi_k;
        if (num % den == 0) {
            /* need room for one more term */
            if (cnt == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *new_buf = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                if (!new_buf) abort();           /* memory exhaustion */
                cache = new_buf;
                cap = new_cap;
            }
            cache[cnt++] = k;
        }
        start++;
    }

    return cache[n - 1];
}
