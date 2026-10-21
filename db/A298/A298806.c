/*
 * A298806  Growth series for group with presentation < S, T : S^3 = T^6 =
 *          (S*T)^6 = 1 >.
 * https://oeis.org/A298806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) + 2*a(n-4) - 3*a(n-5) +
 *            2*a(n-6) + a(n-7) - 2*a(n-8) + 3*a(n-9) - a(n-10) for n > 10
 *            Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=25, a(4)=60,
 *            a(5)=148, a(6)=358, a(7)=869, a(8)=2106, a(9)=5110, a(10)=12396
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#  include <new>
#  include <utility>
#else
#  include <stdlib.h>
#endif

/* 10 preceding terms needed for the recurrence.  */
static const seq_t init_vals[11] = {
    (seq_t)1,  (seq_t)4,  (seq_t)10,
    (seq_t)25, (seq_t)60, (seq_t)148,
    (seq_t)358,(seq_t)869,(seq_t)2106,
    (seq_t)5110,(seq_t)12396
};

static seq_t *cache = NULL;   /* memoisation array   */
static size_t cap   = 0;      /* current allocated size */

/* allocate a new array of seq_t, default‑constructed (C++) or zeroed (C) */
static seq_t* alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

/* free an array allocated by alloc_seq_array */
static void free_seq_array(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n < 11U) return init_vals[(size_t)n];

    size_t req = (size_t)n + 1U;          /* need indices 0 … n inclusive */
    if (cap < req) {
        size_t old_cap = cap;
        seq_t* new_cache = NULL;

        if (old_cap == 0) {                /* first allocation                */
            new_cache = alloc_seq_array(req);
            cap = req;
            /* initialise the known prefix */
            for (size_t i = 0; i < 11U; ++i)
                new_cache[i] = init_vals[i];
        } else {
            /* grow the array */
#ifdef __cplusplus
            new_cache = new seq_t[req];
            for (size_t i = 0; i < old_cap; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
#else
            new_cache = (seq_t*)realloc(cache, req * sizeof(seq_t));
#endif
            cache = new_cache;
            cap   = req;
        }

        cache = new_cache; /* update for C++ new, otherwise already set */

        /* compute new terms using the recurrence */
        size_t start = (old_cap > 11U) ? old_cap : 11U;
        for (size_t i = start; i < req; ++i) {
            cache[i] =
                3 * cache[i-1] - 2 * cache[i-2] + cache[i-3]
              + 2 * cache[i-4] - 3 * cache[i-5] + 2 * cache[i-6]
              + cache[i-7]      - 2 * cache[i-8] + 3 * cache[i-9]
              - cache[i-10];
        }
    }

    return cache[(size_t)n];
}
