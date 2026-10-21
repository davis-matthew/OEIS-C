/*
 * A245942  Irregular triangle read by rows of a variation of the Collatz
 *          iteration with signature (2,3).
 * https://oeis.org/A245942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A245942: irregular triangle rows of the Collatz-like
   iteration with signature (2,3).
   The sequence is read by rows: each row is the trajectory of the
   iteration beginning with consecutive integers 5,6,7,…; the
   iteration stops when the value 16 is reached.  The returned value
   a(n) is the n-th term of the concatenated rows, 1‑based.  */

#include <stdlib.h>
#include <stdbool.h>

static inline seq_t collatz_next(seq_t x)
{
    return (x & 1) ? (x * 3 - 5) : (x / 2 + 2);
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* next starting value for the next row (row 1 starts with 5) */
    static idx_t next_start = 5;

    size_t target = (size_t)n;          /* 1‑based target index */

    /* Generate rows until we have at least `n` terms */
    while (cache_len < target) {
        seq_t val = (seq_t) next_start;

        /* Append the first value of the new row */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = val;

        /* Continue the iteration until 16 is reached,
           at least one step is performed even if start == 16 */
        do {
            val = collatz_next(val);
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        } while (val != 16);

        ++next_start;   /* prepare the next row */
    }

    return cache[n - 1];
}
