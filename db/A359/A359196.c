/*
 * A359196  a(n) is the number of subsets of the divisors of n which sum to
 *          n+1.
 * https://oeis.org/A359196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 719, u32 n <= 2519, u64 n <= 9755,
 *            u128 n <= 9755, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A359196 – number of divisor subsets summing to n+1
 *  This implementation builds the list of all divisors of n,
 *  then uses a standard bounded knapsack (subset‑sum) DP to count
 *  how many subsets of those divisors reach the target sum n+1.
 *  Complexity:  O(τ(n)·(n+1))  where τ(n) is the divisor count.
 *  Works for n up to at least 10 000 in well under two seconds. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* only defined for n ≥ 1 */

    const idx_t target = n + 1;       /* the required subset sum */

    /* ---- collect all divisors of n ------------------------------------- */
    size_t cap = 16;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs)
        return 0;                    /* allocation failure -> 0 as fallback */

    size_t len = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (len == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) {
                    free(divs);
                    return 0;
                }
                divs = tmp;
            }
            divs[len++] = i;

            idx_t j = n / i;
            if (j != i) {
                if (len == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) {
                        free(divs);
                        return 0;
                    }
                    divs = tmp;
                }
                divs[len++] = j;
            }
        }
    }

    /* ---- subset‑sum DP: count ways to reach each sum up to target ----- */
    size_t dp_size = (size_t)target + 1;              /* indices 0 .. target */
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) {
        free(divs);
        return 0;
    }
    dp[0] = 1;                                         /* empty subset */

    for (size_t t = 0; t < len; ++t) {
        idx_t d = divs[t];
        for (idx_t s = target; true; --s) {            /* safe descending */
            dp[s] += dp[s - d];
            if (s == d) break;
        }
    }

    seq_t result = dp[target];

    free(dp);
    free(divs);
    return result;
}
