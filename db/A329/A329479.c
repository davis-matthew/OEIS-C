/*
 * A329479  Number of degree n polynomials f with all nonzero coefficients
 *          equal to 1 such that f(k) is divisible by 3 for all integers k.
 * https://oeis.org/A329479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329479: Number of degree‑n polynomials f with all
 * non‑zero coefficients equal to 1 such that f(k) is divisible by 3
 * for all integers k.
 *
 *  a(n) satisfies the 8‑term linear recurrence (for n ≥ 9)
 *
 *      a(n) = 2·a(n‑1) + 2·a(n‑2) – 5·a(n‑3) – 2·a(n‑4)
 *             +10·a(n‑5) – 4·a(n‑6) – 4·a(n‑7) + 8·a(n‑8)
 *
 *  with initial values
 *
 *      a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *      a(5)=1, a(6)=2, a(7)=6, a(8)=15.
 *
 *  The implementation memoises results in a static array so that
 *  calls may appear in any order.  The array is grown only when a
 *  larger index is requested and is always at least 9 elements long
 *  to hold the initial values.  All arithmetic uses the user‑defined
 *  type seq_t; the code is fully portable to the 64‑bit and
 *  arbitrary‑precision builds. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation: cache[0] is unused.                     */
    static seq_t *cache  = NULL;   /* array of seq_t values            */
    static size_t cache_cap = 0;  /* capacity (in elements)           */
    static idx_t  cache_max = 0;  /* largest index currently stored    */

    if (n < 1) return 0;  /* the sequence is defined for n ≥ 1 */

    /* Ensure cache is large enough.  It must always be at least 9 elements
     * so that the base cases can be stored.  For a newly requested n
     * we allocate (n+1) elements, but never fewer than 9.               */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 9) new_cap = 9;

    if (new_cap > cache_cap) {
        /* Grow the array.  realloc keeps existing contents.           */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base terms when the cache was just created.          */
    if (cache_max == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)2;
        cache[7] = (seq_t)6;
        cache[8] = (seq_t)15;
        cache_max = 8;
    }

    /* Compute any missing terms up to n.                              */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t pos = 2 * cache[i - 1]
                      + 2 * cache[i - 2]
                      + 10 * cache[i - 5]
                      + 8  * cache[i - 8];
            seq_t neg = 5 * cache[i - 3]
                      + 2 * cache[i - 4]
                      + 4 * cache[i - 6]
                      + 4 * cache[i - 7];
            cache[i] = pos - neg;       /* all terms are non‑negative      */
        }
        cache_max = n;
    }

    return cache[n];
}
