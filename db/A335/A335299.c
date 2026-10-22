/*
 * A335299  a(0) = 0; for n > 0, a(n) = abs(a(n-1) - n) if not already in the
 *          sequence, otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A335299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335299:
   a(0) = 0;
   for n > 0:
       let cand = |a(n-1) - n|
       if cand has not appeared earlier in the sequence
           a(n) = cand
       else
           a(n) = a(n-1) + n
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoized cache of known terms */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* Create the first cache entry if needed */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out‑of‑memory fallback */
        cache[0] = (seq_t)0;
        max_cached = 0;
    }

    /* Compute missing values up to n */
    if (n > max_cached) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* out‑of‑memory fallback */
        cache = tmp;

        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t nn   = (seq_t)i;

            /* candidate = |prev - nn| */
            seq_t cand = (prev >= nn ? prev - nn : nn - prev);

            /* search for cand in the already computed part of the sequence */
            bool found = false;
            for (idx_t j = 0; j < i; ++j) {
                if (cache[j] == cand) {
                    found = true;
                    break;
                }
            }

            cache[i] = found ? (prev + nn) : cand;
        }
        max_cached = n;
    }

    return cache[n];
}
