/*
 * A248462  Number of length 1+2 0..n arrays with no three consecutive terms
 *          having the sum of any two elements equal to twice the third.
 * https://oeis.org/A248462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5)
 *            Initial values: a(1)=6, a(2)=18, a(3)=48, a(4)=96, a(5)=174
 *            Recurrence applies for n >= 6. The function uses simple memoised
 *            DP to handle any ordering of calls
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage as requested ------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache for memoisation ------------------------------------------------- */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;        /* number of entries cached (highest n available) */

    /* ensure we have room for n (1‑based indexing) ---------------------------------- */
    if (n > cache_n) {
        size_t new_size = n + 1;     /* index 0 unused, store at cache[i] for i>=1 */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* initialise new entries */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)6;
            else if (i == 2) cache[i] = (seq_t)18;
            else if (i == 3) cache[i] = (seq_t)48;
            else if (i == 4) cache[i] = (seq_t)96;
            else if (i == 5) cache[i] = (seq_t)174;
            else {
                /* recurrence: a(i) = 3*a(i-1) + 3*a(i-4) - (2*a(i-2) + 2*a(i-3) + a(i-5)) */
                seq_t term1 = 3*cache[i-1] + 3*cache[i-4];
                seq_t sub    = 2*cache[i-2] + 2*cache[i-3] + cache[i-5];
                cache[i] = term1 - sub;
            }
        }
        cache_n = n;
    }
    return cache[n];
}
