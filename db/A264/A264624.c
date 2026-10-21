/*
 * A264624  Number of (n+1)X(4+1) arrays of permutations of 0..n*5+4 with each
 *          element having directed index change -2,0 -1,0 0,-1 or 1,1.
 * https://oeis.org/A264624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the empirical linear recurrence * a(n) = 3*a(n-1)
 *            + 36*a(n-3) + 108*a(n-4) * + 729*a(n-11) * - 36*a(n-5) -
 *            351*a(n-6) - 1863*a(n-7) * - 4698*a(n-8) - 4050*a(n-9) -
 *            243*a(n-10) * for n > 12, with the initial values * a(1)=0,
 *            a(2)=9, a(3)=40, a(4)=108, a(5)=660, * a(6)=4032, a(7)=195...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoised values; index 0 unused. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated elements (>= needed + 1) */
    static idx_t   max_n  = 0;   /* highest n computed so far */

    /* Base values for n = 1..12.  The array is 13 elements long
     * so that the indices match the sequence indices directly. */
    static const seq_t base_vals[13] = {
        (seq_t)0,          /* 0 (unused) */
        (seq_t)0,          /* a(1)  */
        (seq_t)9,          /* a(2)  */
        (seq_t)40,         /* a(3)  */
        (seq_t)108,        /* a(4)  */
        (seq_t)660,        /* a(5)  */
        (seq_t)4032,       /* a(6)  */
        (seq_t)19521,      /* a(7)  */
        (seq_t)89469,      /* a(8)  */
        (seq_t)449550,     /* a(9)  */
        (seq_t)2308554,    /* a(10) */
        (seq_t)11454129,   /* a(11) */
        (seq_t)56190429    /* a(12) */
    };

    if (n < 1)          /* behaviour below the first defined term is irrelevant */
        return (seq_t)0;

    /* Allocate or grow the cache if necessary. */
    if (cap <= (size_t)n) {
        size_t new_cap = (cap > 0 ? cap * 2 : 8);
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure → return 0 (unlikely) */
        cache = (seq_t *)tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n, reusing previously computed ones. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 12) {
            /* Base case: use the predefined values. */
            cache[i] = base_vals[i];
        } else {
            /* Recurrence: compute using only additions and subtractions to
             * avoid underflow with unsigned arithmetic. */
            seq_t res = 0;
            res += (seq_t)3   * cache[i - 1];
            res += (seq_t)36  * cache[i - 3];
            res += (seq_t)108 * cache[i - 4];
            res += (seq_t)729 * cache[i - 11];
            res -= (seq_t)36  * cache[i - 5];
            res -= (seq_t)351 * cache[i - 6];
            res -= (seq_t)1863* cache[i - 7];
            res -= (seq_t)4698* cache[i - 8];
            res -= (seq_t)4050* cache[i - 9];
            res -= (seq_t)243 * cache[i - 10];
            cache[i] = res;
        }
    }
    max_n = n;
    return cache[n];
}
