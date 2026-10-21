/*
 * A283772  Numbers k such that U(k) = 0 mod 3, where U = A001950 = upper
 *          Wythoff sequence.
 * https://oeis.org/A283772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 9059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283772: numbers k such that U(k) = 0 (mod 3),
   where U = A001950 is the upper Wythoff sequence.
   U(k) = floor(k*φ) + k  (φ = (1+√5)/2).
   The function builds the sequence by scanning k upward until the
   n‑th qualifying value is found, memoising results for
   subsequent calls.  No floating point or big‑int arithmetic
   is performed on seq_t values. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, return 0 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* holds found k values */
    static size_t cap   = 0;      /* allocated capacity of cache */
    static size_t len   = 0;      /* number of elements stored */
    static idx_t next_k = 1;      /* next k to test */

    /* we keep a simple growing array, doubling capacity when needed */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we have the n-th value */
    while (len < n) {
        /* compute floor(k*φ) using double arithmetic only for the
           conversion; this is safe because k is < ~1e6 for the first
           many terms and double has 53‑bit integer precision. */
        static const double phi = 1.618033988749895; /* (1+√5)/2 */
        idx_t k = next_k;
        seq_t Lk = (seq_t)((idx_t)((double)k * phi));
        seq_t Uk = Lk + k;                 /* U(k) = floor(k*φ) + k */
        if ((Uk % 3) == 0) {
            /* store value in cache */
            if (len == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
