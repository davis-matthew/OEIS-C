/*
 * A364655  Circuit rank and corank of the n-Pell graph.
 * https://oeis.org/A364655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A364655 –   a(n) = 5*a(n‑1) – 6*a(n‑2) – 2*a(n‑3) + 3*a(n‑4) + a(n‑5)
    initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=7, a(4)=30
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms              */
static size_t cache_size = 0;    /* number of terms cached      */
static size_t cache_cap = 0;     /* current allocation capacity */

/* ----------------------------------------------------------------------
   Computes the n-th term of A364655 using a linear recurrence.
   Results are cached so that subsequent calls are O(1) after an
   initial O(n) build up to the requested index.
----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      needed * sizeof(seq_t));
        if (!tmp) {                     /* allocation failed */
            /* For the purposes of this sequence we simply return 0.
               Real builds should handle this error appropriately. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = needed;
    }

    /* Fill in missing terms */
    for (size_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)1;
        else if (i == 3)
            cache[i] = (seq_t)7;
        else if (i == 4)
            cache[i] = (seq_t)30;
        else {
            seq_t val = 5 * cache[i - 1];          /* 5*a(n-1) */
            val -= 6 * cache[i - 2];              /* – 6*a(n-2) */
            val -= 2 * cache[i - 3];              /* – 2*a(n-3) */
            val += 3 * cache[i - 4];              /* + 3*a(n-4) */
            val += cache[i - 5];                  /* + a(n-5) */
            cache[i] = val;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
