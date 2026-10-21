/*
 * A243028  Number of length n+3 0..3 arrays with no four elements in a row
 *          with pattern abba (possibly a=b) and new values 0..3 introduced in
 *          0..3 order.
 * https://oeis.org/A243028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243028: Number of length n+3 0..3 arrays with no four
   consecutive elements in the pattern abba, new values 0..3 introduced in
   0..3 order.
   Empirical recurrence (valid for n≥7):
        a(n) = 4*a(n-1) - a(n-2) + 2*a(n-3) - 6*a(n-4) - 2*a(n-5) - 3*a(n-6)
   Initial values:
        a(1)=13, a(2)=42, a(3)=147, a(4)=536,
        a(5)=1987, a(6)=7414. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"  /* for ensure_capacity */

static seq_t *cache = NULL;      /* index 0 unused, indices 1..n used */
static size_t  capacity = 0;     /* allocated size */
static size_t  cached   = 0;     /* highest n computed (size of stored data) */

seq_t a(idx_t n)
{
    /* growth: if requested n exceeds cache size, extend cache and compute
       missing values using recurrence. */
    if ((size_t)n > cached) {
        size_t want = (size_t)n + 1;   /* indices 0..n inclusive */
        ensure_capacity(want, &capacity, (void**)&cache, sizeof(seq_t));

        /* initial values for n=1..6 (cache[0] unused) */
        static const seq_t init[7] = { 0,
            13ULL, 42ULL, 147ULL, 536ULL,
            1987ULL, 7414ULL
        };

        for (idx_t i = (idx_t)(cached + 1); i <= n; ++i) {
            seq_t v;
            if (i <= 6) {
                v = init[i];
            } else {
                /* order terms to avoid temporary underflows: */
                v = 4 * cache[i - 1];
                v = v - cache[i - 2];
                v = v + 2 * cache[i - 3];
                v = v - 6 * cache[i - 4];
                v = v - 2 * cache[i - 5];
                v = v - 3 * cache[i - 6];
            }
            cache[i] = v;
        }
        cached = (size_t)n;
    }

    return cache[n];
}
