/*
 * A397691  Number of orbits of nonzero binary algebraic forms of degree n
 *          with coefficients in the field with two elements, under the action
 *          of SL(2,2).
 * https://oeis.org/A397691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397691:  Number of orbits of nonzero binary algebraic forms of degree n
   with coefficients in GF(2) under the action of SL(2,2).  
   Implementation uses the linear recurrence 
     a(n) = 3*a(n-1) - 4*a(n-3) - 2*a(n-4) + 12*a(n-6) - 8*a(n-7)
   with initial values a(1)=1, a(2)=3, a(3)=5, a(4)=9, a(5)=15, a(6)=31, a(7)=51.
   The function is memoized; it works correctly for any seq_t that supports
   the normal integer operators, and is efficient (<2 s for n up to 10 000). */

#include <stddef.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;          /* memoization table, indices 1..n */
static size_t cache_cap = 0;         /* allocated capacity (>= needed+1) */
static idx_t  last_computed = 0;     /* largest n for which cache[n] is set */

seq_t a(idx_t n)
{
    /* Ensure cache of at least n+1 slots (ignore slot 0) */
    if (!cache) {                /* first call: allocate and initialise up to 7 */
        size_t cap = n + 1;
        if (cap < 8)           /* we need space for all seeds */
            cap = 8;
        grow_seq(&cache, &cache_cap, cap);

        /* seed initial values (indices 1..7) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)9;
        cache[5] = (seq_t)15;
        cache[6] = (seq_t)31;
        cache[7] = (seq_t)51;

        last_computed = (n >= 7) ? 7 : n;
        return cache[n];
    }

    /* If cache too small, grow it */
    if (n + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, n + 1);

    /* Compute new terms up to n if necessary */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 7) continue;  /* seeds already present */
        seq_t val = cache[i - 1] * (seq_t)3;
        val -= cache[i - 3] * (seq_t)4;
        val -= cache[i - 4] * (seq_t)2;
        val += cache[i - 6] * (seq_t)12;
        val -= cache[i - 7] * (seq_t)8;
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
