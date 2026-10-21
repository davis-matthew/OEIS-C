/*
 * A292509  Primes of the form k^2 + 23*k + 23.
 * https://oeis.org/A292509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form k^2 + 23*k + 23 (OEIS A292509),
   with offset 1: a(1) = 23, a(2) = 47, … This implementation does
   a memoised incremental search using the helper isPrimeGeneric. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t n); */

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Cache of already found values. */
    static seq_t *cache = NULL;   /* array of cached values          */
    static idx_t cached   = 0;    /* number of values stored         */
    static idx_t cap      = 0;    /* current capacity of the cache   */
    static idx_t last_k   = 0;    /* last k value examined           */

    if (n == 0)   /* out of bounds – caller should not request n<1 */
        return 0;

    /* Allocate or grow cache as needed. */
    if (cached < n) {
        if (cache == NULL) {
            cap = 32;
            cache = (seq_t *)malloc(sizeof(seq_t) * cap);
            if (cache == NULL) return 0;   /* allocation failure (rare) */
        } else if (cached >= cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            if (tmp == NULL) return 0;     /* allocation failure (rare) */
            cache = tmp;
        }

        /* Continue searching until the required number of terms is found. */
        while (cached < n) {
            seq_t kk = (seq_t)last_k;
            seq_t val = kk * kk + (seq_t)23 * kk + (seq_t)23;
            if (isPrimeGeneric(val)) {
                if (cached >= cap) {        /* buffer full – expand */
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
                    if (tmp == NULL) return 0;
                    cache = tmp;
                }
                cache[cached++] = val;
            }
            ++last_k;
        }
    }
    return cache[n - 1];
}
