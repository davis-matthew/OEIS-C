/*
 * A378110  Cogrowth sequence of the 18-element group S3:C3 = <S,T,U | S^3,
 *          T^3, U^2, (SU)^2, (TU)^2, [S,T]>.
 * https://oeis.org/A378110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378110 – Cogrowth of the 18‑element group  
   Recurrence (order 5 derived from the generating function):  
   a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=5, a(5)=10  
   a(n)=6*a(n-2)+10*a(n-3)+3*a(n-4)-18*a(n-5)   for n≥5 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;    /* cached values 0 .. cache_size-1 */
static idx_t cache_size = 0;   /* number of elements currently cached */

/* Initial six terms of the sequence */
static const seq_t initial[6] = {
    (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)10
};

/* Compute a(n), memoising previously computed values */
seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n < 6)
        return initial[n];

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_size = 6;                      /* indices 0–5 stored */
        cache = alloc_seq_array(cache_size);
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = initial[i];
    }

    /* Grow the cache if the requested index is beyond current size */
    if (n >= cache_size) {
        idx_t new_size = n + 1;              /* need indices 0..n */
        seq_t *new_cache = alloc_seq_array(new_size);

        /* Copy previously cached values to the new array */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Free old storage and make the new cache the active one */
        free_seq(cache);
        cache = new_cache;
        idx_t old_size = cache_size;        /* remember old boundaries */
        cache_size = new_size;

        /* Compute new terms using the recurrence, starting after the old data */
        for (idx_t i = old_size; i < new_size; ++i) {
            /* a(i) = 6*a(i-2)+10*a(i-3)+3*a(i-4)-18*a(i-5)  (i ≥ 5) */
            seq_t term =
                cache[i-2] * (seq_t)6 +
                cache[i-3] * (seq_t)10 +
                cache[i-4] * (seq_t)3  -
                cache[i-5] * (seq_t)18;
            cache[i] = term;
        }
    }

    return cache[n];
}
