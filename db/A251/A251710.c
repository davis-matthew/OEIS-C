/*
 * A251710  7-step Fibonacci sequence starting with (0,0,0,0,0,1,0).
 * https://oeis.org/A251710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_array_destroy, seq_array_resize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251710: 7‑step Fibonacci
   Recurrence: a(n+7) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)
   Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1,a(6)=0
   The algorithm keeps a cache of already computed terms and extends it
   only as far as needed.  It works for any integer type seq_t,
   including an arbitrary‑precision integer class.  The allocation
   strategy uses `new[]` in C++ and `malloc`/`realloc` in C; all
   constructed objects are properly destroyed when the cache grows.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_array_destroy.h"
#include "../utils/seq_array_resize.h"

/* ------------------------------------------------------------ */
/*  External linkage function required by the harness           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Use size_t for array indices – n is guaranteed to fit in idx_t */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t cur   = 0;   /* number of terms currently stored, i.e. cur==cache size */

    /* Desired number of terms (index n inclusive) */
    size_t needed = (size_t)n + 1;

    /* Initialise cache with the first 7 terms if it has not been allocated yet. */
    if (cache == NULL) {
        cap = 7;
        cache = seq_array_resize(NULL, 0, cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)0;
        cur = 7;                    /* terms 0..6 are now available */
    }

    /* Grow the cache if necessary.  We grow by a factor of 2 to keep
       amortised O(1) per call. */
    if (needed > cap) {
        size_t newcap = cap * 2;
        if (newcap < needed) newcap = needed;
        cache = seq_array_resize(cache, cap, newcap);
        cap = newcap;
    }

    /* Compute new terms up to index n if not already done. */
    if (needed > cur) {
        /* Compute sum of the last seven terms to start the recurrence. */
        seq_t sum = (seq_t)0;
        if (cur >= 7) {
            /* Sum cache[cur-7] .. cache[cur-1] */
            for (size_t j = cur - 7; j < cur; ++j) sum += cache[j];
        } else {
            /* For cur < 7, just sum the already known terms. */
            for (size_t j = 0; j < cur; ++j) sum += cache[j];
        }

        /* Generate terms from cur up to needed-1. */
        for (size_t i = cur; i < needed; ++i) {
            if (i < 7) {
                /* The first seven values are already known; this branch
                   only serves to keep the compiler quiet about unused
                   variables. */
                switch (i) {
                    case 0: cache[i] = (seq_t)0; break;
                    case 1: cache[i] = (seq_t)0; break;
                    case 2: cache[i] = (seq_t)0; break;
                    case 3: cache[i] = (seq_t)0; break;
                    case 4: cache[i] = (seq_t)0; break;
                    case 5: cache[i] = (seq_t)1; break;
                    case 6: cache[i] = (seq_t)0; break;
                }
            } else {
                /* Recurrence: new term = sum of previous seven terms. */
                cache[i] = sum;
                /* Update sliding window sum for the next term. */
                sum += cache[i];
                sum -= cache[i - 7];
            }
        }
        cur = needed;
    }

    return cache[(size_t)n];
}
