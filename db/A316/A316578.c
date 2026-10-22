/*
 * A316578  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 13*a(n-2) + 12*a(n-3) - 18*a(n-4) - 27*a(n-5) +
 *            4*a(n-6) + 19*a(n-7) + 31*a(n-8) - 3*a(n-10) - 3*a(n-11) -
 *            14*a(n-12) - 6*a(n-14) + 4*a(n-15) for n>16 Initial terms:
 *            a(1)=1, a(2)=23, a(3)=63, a(4)=338, a(5)=1312, a(6)=5936,
 *            a(7)=25271, a(8)=110555, a(9)=478290, a(10)=2079671, a(11...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values; grows to accommodate any n requested   */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;          /* current size of memo (indices 0..memo_len-1) */

    /* initialise cache with the first 16 terms */
    if (memo_len == 0) {
        memo_len = 17;                  /* indices 0..16 (we use 1..16) */
        memo = (seq_t*)malloc((size_t)memo_len * sizeof(seq_t));

        /* 0-index unused */
        memo[ 1] = 1;
        memo[ 2] = 23;
        memo[ 3] = 63;
        memo[ 4] = 338;
        memo[ 5] = 1312;
        memo[ 6] = 5936;
        memo[ 7] = 25271;
        memo[ 8] = 110555;
        memo[ 9] = 478290;
        memo[10] = 2079671;
        memo[11] = 9022567;
        memo[12] = 39184505;
        memo[13] = 170094043;
        memo[14] = 738520371;
        memo[15] = 3206190960;
        memo[16] = 13919960703;
    }

    /* return precomputed value if already available */
    if (n < memo_len)
        return memo[n];

    /* extend memo array to hold up to n */
    idx_t new_len = n + 1;                 /* allocate indices 0..n */
    memo = (seq_t*)realloc(memo, (size_t)new_len * sizeof(seq_t));

    /* compute new terms using the recurrence */
    for (idx_t i = memo_len; i <= n; ++i) {
        seq_t pos = memo[i-1] + 13*memo[i-2] + 12*memo[i-3]
                    + 4*memo[i-6]     + 19*memo[i-7] + 31*memo[i-8]
                    + 4*memo[i-15];
        seq_t neg = 18*memo[i-4] + 27*memo[i-5]
                    + 3*memo[i-10] + 3*memo[i-11] + 14*memo[i-12]
                    + 6*memo[i-14];
        memo[i] = pos - neg;
    }

    memo_len = new_len;
    return memo[n];
}
