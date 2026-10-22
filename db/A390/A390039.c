/*
 * A390039  a(n) = Sum_{k=0..floor(4*n/7)} binomial(k+3,4*n-7*k).
 * https://oeis.org/A390039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 59, u64 n <= 119,
 *            u128 n <= 239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A390039
   Formula: a(n) = Sum_{k=0..floor(4*n/7)} binomial(k+3,4*n-7*k)
   Recurrence: a(n) = 4*a(n-2) - 6*a(n-4) + 4*a(n-6) + a(n-7) - a(n-8)
   Initial values (n = 0..7): 1,0,4,0,10,0,20,1
   The function is memoised so that any order of calls works.
*/
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cap      = 0;      /* allocated capacity */
    static size_t len      = 0;      /* number of elements currently in cache */

    /* initial values for n < 8 */
    static const seq_t base[8] = {1,0,4,0,10,0,20,1};

    /* ------------------------------------------------------------------ */
    /*  1. Ensure that the cache can hold n+1 elements                     */
    /* ------------------------------------------------------------------ */
    if (n >= len) {
        /* enlarge the cache if necessary */
        if (n + 1 > cap) {
            size_t new_cap = (cap == 0) ? 16 : cap << 1;
            while (new_cap <= (size_t)n)
                new_cap <<= 1;

            /* allocation compatible with C and C++ */
#ifdef __cplusplus
            /* operator new[] returns a pointer to uninitialised storage */
            void *tmp = ::operator new(new_cap * sizeof(seq_t));
#else
            void *tmp = malloc(new_cap * sizeof(seq_t));
#endif
            if (!tmp) abort();      /* out of memory */

            /* move any existing data */
            if (cache) {
                /* cast to char* for memcpy */
                memcpy(tmp, cache, len * sizeof(seq_t));
#ifdef __cplusplus
                /* destroy old array */
                ::operator delete[](cache);
#else
                free(cache);
#endif
            }
            cache = (seq_t*)tmp;
            cap   = new_cap;
        }

        /* ---------------------------------------------------------------- */
        /*  2. Initialise first elements if this is the first call          */
        /* ---------------------------------------------------------------- */
        if (len == 0) {
            for (size_t i = 0; i < sizeof(base)/sizeof(base[0]) && i < cap; ++i)
                cache[i] = base[i];
            len = sizeof(base)/sizeof(base[0]) > cap ? cap : sizeof(base)/sizeof(base[0]);
        }

        /* ---------------------------------------------------------------- */
        /*  3. Compute further elements up to n (if required)                */
        /* ---------------------------------------------------------------- */
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t val;
            if (i < 8) {
                val = base[i];
            } else {
                /* recurrence (all operations are exact on seq_t) */
                val = 4*cache[i-2]
                    - 6*cache[i-4]
                    + 4*cache[i-6]
                    + cache[i-7]
                    - cache[i-8];
            }
            cache[i] = val;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
