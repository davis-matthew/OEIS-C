/*
 * A398666  Upper (1/2, 2) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6) with a(0)=0, a(1)=3, a(2)=12, a(3)=31, a(4)=65, a(5)=118
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define MAX_CACHE 10001   /* up to n≈10000, adjust if larger N is expected */

static seq_t cache[MAX_CACHE];
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise the first six terms once */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)65;
        cache[5] = (seq_t)118;
        cache_size = 6;
    }

    /* Handle small indices directly */
    if (n < 6)
        return cache[n];

    /* Allow only the supported range.  For larger n the caller can
       extend MAX_CACHE if needed.  For safety we limit computation to
       MAX_CACHE-1. */
    if (n >= MAX_CACHE) {
        /* Compute on-the-fly for n beyond the static cache.  We do not
           keep intermediate values in memory to avoid dynamic allocation
           of large seq_t objects.  The recurrence uses only the last six
           values. */
        seq_t v0 = cache[cache_size-6];
        seq_t v1 = cache[cache_size-5];
        seq_t v2 = cache[cache_size-4];
        seq_t v3 = cache[cache_size-3];
        seq_t v4 = cache[cache_size-2];
        seq_t v5 = cache[cache_size-1];
        seq_t vi;
        for (idx_t i = cache_size; i <= n; ++i) {
            vi = 4*v5 - 7*v4 + 8*v3 - 7*v2 + 4*v1 - v0;
            v0 = v1; v1 = v2; v2 = v3; v3 = v4; v4 = v5; v5 = vi;
        }
        return vi;
    }

    /* Extend the static cache up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 4*cache[i-1] - 7*cache[i-2] + 8*cache[i-3]
                  - 7*cache[i-4] + 4*cache[i-5] - cache[i-6];
    }
    cache_size = n + 1;
    return cache[n];
}
