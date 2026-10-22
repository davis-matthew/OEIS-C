/*
 * A373013  Number of distinct partitions p of n such that max(p) == 2 mod 3.
 * https://oeis.org/A373013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct partitions of n whose maximum part is
 *            congruent to 2 modulo 3 (OEIS A373013)
 * Range:     u8 n <= 37, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *dp = NULL;      /* dp[s] = # of distinct partitions of s  */
static seq_t *ans = NULL;     /* ans[s] = # of distinct partitions of s
                                 whose maximum part ≡ 2 (mod 3)       */
static idx_t max_size = 0;    /* current allocation size */

/* --------------------------------------------------------------------- */
/* a: compute the n‑th term of the sequence                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have enough space for indices 0 .. n                 */
    size_t need = (size_t)n + 1;
    if (need > max_size) {
        dp   = (seq_t *)realloc(dp,   need * sizeof(seq_t));
        ans  = (seq_t *)realloc(ans,  need * sizeof(seq_t));
        max_size = need;
    }

    /* Initialise arrays for this freshly computed range              */
    dp[0] = 1;      /* empty partition                            */
    ans[0] = 0;     /* no partition has a defined maximum         */
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = 0;
        ans[i] = 0;
    }

    /* Dynamic programming for distinct partitions                    */
    for (idx_t part = 1; part <= n; ++part) {
        if (part % 3 == 2) {               /* part contributes to 2 (mod 3)      */
            for (idx_t sum = n; sum >= part; --sum) {
                seq_t add = dp[sum - part];
                dp[sum] += add;
                ans[sum] += add;          /* new maximum part is part           */
            }
        } else {                           /* part does not affect the answer   */
            for (idx_t sum = n; sum >= part; --sum) {
                dp[sum] += dp[sum - part];
            }
        }
    }

    return ans[n];
}
