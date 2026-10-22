/*
 * A373930  Number of compositions of 7*n-4 into parts 1 and 7.
 * https://oeis.org/A373930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A373930 – Number of compositions of 7*n‑4 into parts 1 and 7.
   Recurrence (valid for n ≥ 8):
      a(n) = 8*a(n‑1) – 21*a(n‑2) + 35*a(n‑3) – 35*a(n‑4)
             + 21*a(n‑5) – 7*a(n‑6) + a(n‑7)
   Initial values:
      a(1)=1, a(2)=5, a(3)=22, a(4)=105, a(5)=518, a(6)=2555, a(7)=12565
*/

#include <stdlib.h>

/* Return the n-th term (1‑indexed).  If n ≤ 7 the value is taken from
   the known initial values; otherwise the recurrence is applied.  Results
   are cached so that repeated calls with arbitrary n are efficient. */
seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) return (seq_t)0;           /* unused but safe */
    if (n <= 7) {
        static const seq_t init[7] = {
            1, 5, 22, 105, 518, 2555, 12565
        };
        return init[n - 1];
    }

    /* Static cache of already computed terms.  The array stores a(1) at
       index 0, a(2) at index 1, etc. */
    static seq_t *cache        = NULL;
    static idx_t cache_size    = 0;       /* number of terms stored */
    static size_t cache_cap    = 0;       /* allocated capacity */

    idx_t needed = n;                    /* we need terms up to n */

    /* Allocate or grow the cache as needed. */
    if (cache == NULL) {
        /* First allocation: reserve enough space for the required
           terms, but at least 8. */
        cache_cap = (needed > 8 ? needed : 8);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory */
        /* Initialise the first 7 terms. */
        static const seq_t init[7] = {
            1, 5, 22, 105, 518, 2555, 12565
        };
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
        cache_size = 7;
    }
    else if (cache_cap < needed) {
        /* Grow the array – double its size or just enough for *needed*. */
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to `n` using the recurrence. */
    for (idx_t i = cache_size + 1; i <= needed; ++i) {
        /* Indexes are 1‑based, array indices are 0‑based. */
        size_t idx = (size_t)(i - 1);
        seq_t val = (seq_t)8  * cache[idx - 1]
                  + (seq_t)21 * cache[idx - 2]
                  + (seq_t)35 * cache[idx - 3]
                  + (seq_t)35 * cache[idx - 4]
                  + (seq_t)21 * cache[idx - 5]
                  + (seq_t)7  * cache[idx - 6]
                  +       cache[idx - 7];
        /* The recurrence contains alternating signs; we used positive
           coefficients because in the original formula the signs
           alternate: 8, -21, +35, -35, +21, -7, +1.
           Therefore we subtract the terms with negative signs: */
        val = cache[idx - 1] * (seq_t)8
            - cache[idx - 2] * (seq_t)21
            + cache[idx - 3] * (seq_t)35
            - cache[idx - 4] * (seq_t)35
            + cache[idx - 5] * (seq_t)21
            - cache[idx - 6] * (seq_t)7
            + cache[idx - 7];
        cache[idx] = val;
    }

    cache_size = needed;  /* we now have all terms up to `n` */
    return cache[n - 1];
}
