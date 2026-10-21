/*
 * A260585  Number of ways to place 2n rooks on an n X n board, with 2 rooks
 *          in each row and each column, multiple rooks in a cell allowed, and
 *          exactly 2 rooks below the main diagonal.
 * https://oeis.org/A260585
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) = 12*a(n-1) - 59*a(n-2) + 155*a(n-3) - 236*a(n-4)
          + 209*a(n-5) - 100*a(n-6) + 20*a(n-7)
   a(2)=1, a(3)=11, a(4)=72, a(5)=367, a(6)=1630, a(7)=6680, a(8)=26082
   Offset: 2   (a(2) is the first term) */
#include <stdlib.h>

static seq_t *cache   = NULL;      /* Cached values a(0), a(1), … */
static idx_t  cache_size  = 0;     /* Currently allocated size (indices 0 … size-1) */
static idx_t  next_compute = 9;    /* Next index that needs to be computed via recurrence */

/* Base values for n = 2 … 8.  Index 0 and 1 are unused. */
static const seq_t base_vals[9] = {
    0,          /* a(0) – unused */
    0,          /* a(1) – unused */
    1,          /* a(2) */
    11,         /* a(3) */
    72,         /* a(4) */
    367,        /* a(5) */
    1630,       /* a(6) */
    6680,       /* a(7) */
    26082       /* a(8) */
};

seq_t a(idx_t n)
{
    /*  Behavior below the offset is irrelevant; a(0) and a(1) are undefined. */
    if (n < 2)
        return (seq_t)0;

    /* Ensure we have enough space to store index n. */
    if (!cache || (idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp;

        if (!cache)
            tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        if (!tmp)
            return (seq_t)0;   /* allocation failure */

        cache = tmp;

        /* Zero any newly allocated entries */
        if (cache_size < new_size) {
            for (idx_t i = cache_size; i < new_size; ++i)
                cache[i] = (seq_t)0;
        }

        cache_size = new_size;

        /* Initialise base cases up to 8, if those indices exist. */
        for (idx_t i = 2; i <= 8 && i < cache_size; ++i)
            cache[i] = base_vals[i];
    }

    /* Compute missing terms via the recurrence, only if needed. */
    if (n >= next_compute) {
        for (idx_t i = next_compute; i <= n; ++i) {
            cache[i] = (seq_t)12  * cache[i-1]
                     - (seq_t)59  * cache[i-2]
                     + (seq_t)155 * cache[i-3]
                     - (seq_t)236 * cache[i-4]
                     + (seq_t)209 * cache[i-5]
                     - (seq_t)100 * cache[i-6]
                     + (seq_t)20  * cache[i-7];
        }
        next_compute = n + 1;
    }

    return cache[n];
}
