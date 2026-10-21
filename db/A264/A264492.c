/*
 * A264492  Number of (4+1) X (n+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change 2,-1 1,0 2,1 0,-1 -2,-2
 *          or -1,0.
 * https://oeis.org/A264492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 16*a(n-2) + 24*a(n-3) + 16*a(n-4) + 32*a(n-5) +
 *            32*a(n-6) Initial values: a(1) = 12, a(2) = 16, a(3) = 212, a(4)
 *            = 788, a(5) = 4772, a(6) = 23076
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    if (n == 0) return 0;          /* not used, offset starts at 1 */

    if (n > max_computed) {
        size_t new_size = (size_t)n + 1;   /* indices 1..n inclusive */
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1)               cache[i] = (seq_t)12;
            else if (i == 2)          cache[i] = (seq_t)16;
            else if (i == 3)          cache[i] = (seq_t)212;
            else if (i == 4)          cache[i] = (seq_t)788;
            else if (i == 5)          cache[i] = (seq_t)4772;
            else if (i == 6)          cache[i] = (seq_t)23076;
            else
                cache[i] = cache[i-1] +
                           ((seq_t)16) * cache[i-2] +
                           ((seq_t)24) * cache[i-3] +
                           ((seq_t)16) * cache[i-4] +
                           ((seq_t)32) * cache[i-5] +
                           ((seq_t)32) * cache[i-6];
        }
        max_computed = n;
    }

    return cache[n];
}
