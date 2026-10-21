/*
 * A259717  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 or 00000011.
 * https://oeis.org/A259717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 5*a(n-3) + 4*a(n-4) - 4*a(n-5) - 8*a(n-6) -
 *            5*a(n-7) - 3*a(n-8) + 2*a(n-9) + 3*a(n-10) with initial values:
 *            a(1)=54, a(2)=95, a(3)=228, a(4)=392, a(5)=556, a(6)=1219,
 *            a(7)=2330, a(8)=3544, a(9)=6772, a(10)=13549
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 31, u64 n <= 68, u128 n <= 143,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Lazy memoised recurrence */
    static seq_t *dp = NULL;
    static idx_t sz  = 0;   /* allocated size (entries 0 .. sz-1) */
    static idx_t last = 0;  /* largest index initialized */

    if (n < 1) return 0;   /* undefined for n<1 */

    /* Ensure enough storage */
    if (sz <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        dp  = tmp;
        sz  = (idx_t)new_cap;
    }

    /* First ten values (hard‑coded as they are needed before the recurrence) */
    static const seq_t init[11] = {0,
        (seq_t)54,  (seq_t)95,  (seq_t)228, (seq_t)392,
        (seq_t)556, (seq_t)1219,(seq_t)2330,(seq_t)3544,
        (seq_t)6772,(seq_t)13549};

    for (idx_t i = 1;
         i <= n && i <= (idx_t)10;
         ++i)
    {
        if (i > last || dp[i] != init[i])
            dp[i] = init[i];
    }

    if (n <= (idx_t)10) return dp[n];

    /* Compute subsequent terms */
    idx_t start = (last > 10) ? last + 1 : 11;
    if (start < 1) start = 1;

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t val;
#ifdef __cplusplus
        /* If seq_t is an integral type, use an unsigned __int128 accumulator */
        if constexpr (std::is_integral<seq_t>::value) {
            unsigned __int128 v = 0;
            v += (unsigned __int128)dp[i - 2];
            v += (unsigned __int128)dp[i - 3] * 5;
            v += (unsigned __int128)dp[i - 4] * 4;
            v -= (unsigned __int128)dp[i - 5] * 4;
            v -= (unsigned __int128)dp[i - 6] * 8;
            v -= (unsigned __int128)dp[i - 7] * 5;
            v -= (unsigned __int128)dp[i - 8] * 3;
            v += (unsigned __int128)dp[i - 9] * 2;
            v += (unsigned __int128)dp[i -10] * 3;
            val = (seq_t)v;
        } else {
            /* seq_t is an arbitrary‑precision type; use mul_small for safety */
            val = dp[i - 2];
            val += mul_small(dp[i - 3], 5);
            val += mul_small(dp[i - 4], 4);
            val -= mul_small(dp[i - 5], 4);
            val -= mul_small(dp[i - 6], 8);
            val -= mul_small(dp[i - 7], 5);
            val -= mul_small(dp[i - 8], 3);
            val += mul_small(dp[i - 9], 2);
            val += mul_small(dp[i -10], 3);
        }
#else
        /* C implementation using unsigned __int128 for intermediate results */
        unsigned __int128 v = 0;
        v += (unsigned __int128)dp[i - 2];
        v += (unsigned __int128)dp[i - 3] * 5;
        v += (unsigned __int128)dp[i - 4] * 4;
        v -= (unsigned __int128)dp[i - 5] * 4;
        v -= (unsigned __int128)dp[i - 6] * 8;
        v -= (unsigned __int128)dp[i - 7] * 5;
        v -= (unsigned __int128)dp[i - 8] * 3;
        v += (unsigned __int128)dp[i - 9] * 2;
        v += (unsigned __int128)dp[i -10] * 3;
        val = (seq_t)v;
#endif
        dp[i] = val;
    }

    last = n;
    return dp[n];
}
