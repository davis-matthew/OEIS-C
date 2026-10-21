/*
 * A270641  The sequence a of 1's and 2's starting with (1,1,1,1) such that
 *          a(n) is the length of the (n+1)st run of a.
 * https://oeis.org/A270641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   array_alloc, array_free, array_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270641: a(1)=1, a(2)=1, a(3)=1, a(4)=1.  
   Let run k be a consecutive block of equal values in a.
   Run 1 has length 4 and consists of 1’s.  
   For k ≥ 2, run k has length a(k‑1) and its value is  
   1 if k is odd, 2 if k is even.  a(n) is the n‑th term of a. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/array_alloc.h"
#include "../utils/array_free.h"
#include "../utils/array_realloc.h"

#ifdef __cplusplus

#else

#endif

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] is a placeholder; valid terms start at index 1. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* largest index that can be stored (excluding placeholder) */
    static size_t len   = 0;   /* number of terms currently cached (excluding placeholder) */

    static idx_t  run_idx   = 1;       /* current run number (1‑based) */
    static seq_t  remaining = 0;       /* terms left in the current run */
    static seq_t  val       = 0;       /* current run value (1 or 2) */

    /* Initialise on first call. */
    if (cache == NULL) {
        cap     = 16;                          /* initial capacity for terms */
        cache   = array_alloc(cap + 1);        /* +1 for the placeholder */
        if (!cache) return 0;                 /* out of memory */
        cache[0] = 0;                          /* placeholder */

        len     = 0;
        run_idx = 1;                           /* start with run 1 */
        remaining = (seq_t)4;                  /* length of run 1 */
        val    = (seq_t)1;                     /* value of run 1 */
    }

    /* Generate values until the cache contains at least n terms. */
    while (len < (size_t)n) {
        /* If the current run is exhausted, start the next run. */
        if (remaining == 0) {
            ++run_idx;
            /* The length of run k comes from a(k‑1). */
            if (run_idx == 1) {
                remaining = (seq_t)4;         /* run 1 length (initialised above) */
            } else {
                remaining = cache[run_idx - 1];
            }
            val = (run_idx % 2 == 1) ? (seq_t)1 : (seq_t)2;
        }

        /* Grow the cache if necessary. */
        if (len + 1 > cap) {
            size_t newcap = cap * 2;
            seq_t* tmp = array_realloc(cache, len, newcap + 1);
            if (!tmp) return 0; /* out of memory */
            cache = tmp;
            cap   = newcap;
        }

        /* Store the next term. */
        cache[len + 1] = val;
        ++len;
        --remaining;
    }

    return cache[n];
}
