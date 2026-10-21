/*
 * A250525  Number of (n+1)X(6+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n‑1) – 85*a(n‑2) + 294*a(n‑3) – 639*a(n‑4) +
 *            906*a(n‑5) – 839*a(n‑6) + 490*a(n‑7) – 164*a(n‑8) + 24*a(n‑9);
 *            Initial values (n = 1 … 9): 35064, 214812, 896191, 3088923,
 *            9580060, 27910024, 78204775, 213775147, 575554760
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 31, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)35064ULL,
    (seq_t)214812ULL,
    (seq_t)896191ULL,
    (seq_t)3088923ULL,
    (seq_t)9580060ULL,
    (seq_t)27910024ULL,
    (seq_t)78204775ULL,
    (seq_t)213775147ULL,
    (seq_t)575554760ULL
};

seq_t a(idx_t n)
{
    /* Offset is 1. Return the pre‑computed values for n <= 9. */
    if (n <= 9) {
        return init[(idx_t)n - 1];
    }

    /* Static cache for memoisation.  cache[0] holds a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms currently cached */

    /* Grow the cache to hold at least n terms. */
    if (cache_size < n) {
        /* realloc returns void* in C; cast explicitly for C and let C++ accept it. */
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) abort();            /* allocation failure */
        cache = new_cache;

        /* Initialise the first nine terms if they are not yet cached. */
        if (cache_size < 9) {
            for (idx_t i = 0; i < 9; ++i) {
                cache[i] = init[i];
            }
        }

        /* Compute terms from the first missing index up to n. */
        for (idx_t i = cache_size; i < n; ++i) {
            /* i is zero‑based.  The recurrence applies for i >= 9 (i.e. term a(i+1) >= 10). */
            if (i < 9) continue;

            seq_t val =
                cache[i - 1] * (seq_t)14
                - cache[i - 2] * (seq_t)85
                + cache[i - 3] * (seq_t)294
                - cache[i - 4] * (seq_t)639
                + cache[i - 5] * (seq_t)906
                - cache[i - 6] * (seq_t)839
                + cache[i - 7] * (seq_t)490
                - cache[i - 8] * (seq_t)164
                + cache[i - 9] * (seq_t)24;

            cache[i] = val;
        }

        cache_size = n;
    }

    return cache[n - 1];
}
