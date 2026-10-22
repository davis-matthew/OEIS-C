/*
 * A359866  a(n) is the number of k > 0 such that n-1-2*k >= 0 and a(n-1-2*k)
 *          >= a(n-1-k) >= a(n-1).
 * https://oeis.org/A359866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1016, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0,
   a(n) = |{ k>0 : n-1-2*k >= 0  and  a(n-1-2*k) >= a(n-1-k) >= a(n-1) }| */

/* Note: the function works for both C11 and C++17. In C++ the cast of
   realloc's void* return value is required. For the arbitrary‑precision
   build, seq_t is assumed to be a POD or trivially‑constructible type. */

#include <stdlib.h>
#include <stdint.h>  /* for size_t */

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of values */
    static size_t  cap    = 0;      /* allocated capacity */
    static size_t  len    = 0;      /* number of values currently stored */

    /* Convert idx_t to size_t for array indexing */
    size_t idx = (size_t)n;

    /* Ensure we have computed up to 'idx' */
    if (idx >= len) {
        /* Grow capacity if necessary */
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= idx) new_cap <<= 1;   /* double until enough */

        /* realloc must be cast for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;

        /* Compute missing values from current len up to idx */
        for (size_t i = len; i <= idx; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
                continue;
            }

            seq_t a_prev = cache[i - 1];
            size_t maxk  = (i - 1) / 2;
            seq_t cnt    = (seq_t)0;

            for (size_t k = 1; k <= maxk; ++k) {
                seq_t a1 = cache[i - 1 - 2 * k];
                seq_t a2 = cache[i - 1 - k];
                if (a1 >= a2 && a2 >= a_prev) {
                    ++cnt;
                }
            }
            cache[i] = cnt;
        }
        len = idx + 1;
    }

    return cache[idx];
}
