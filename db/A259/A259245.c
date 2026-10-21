/*
 * A259245  Number of (n+1) X (3+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0111.
 * https://oeis.org/A259245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) - 11*a(n-3) - 30*a(n-4), n>4 with
 *            a(1)=48, a(2)=192, a(3)=720, a(4)=2816
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: memo[i] holds a(i) for 1 ≤ i ≤ memo_size   */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    if (n == 0)
        return (seq_t)0;       /* not used, but avoid UB */

    /* Allocate or grow the cache if necessary */
    if (!memo || memo_size < n)
    {
        idx_t new_size = (n > 20 ? n : 20) + 5;   /* a bit of slack */
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0)          /* allocation failure */
            return (seq_t)0;               /* caller must not rely on this */
        memo = tmp;
        if (memo_size == 0)
            memo[0] = (seq_t)0;            /* unused placeholder */
    }

    /* Compute new terms up to n */
    for (idx_t i = memo_size + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)
            val = (seq_t)48;
        else if (i == 2)
            val = (seq_t)192;
        else if (i == 3)
            val = (seq_t)720;
        else if (i == 4)
            val = (seq_t)2816;
        else
            val = (seq_t)2 * memo[i - 1] + (seq_t)12 * memo[i - 2]
                 - (seq_t)11 * memo[i - 3] - (seq_t)30 * memo[i - 4];

        memo[i] = val;
    }

    memo_size = n;
    return memo[n];
}
