/*
 * A345556  Numbers that are the sum of ten cubes in eight or more ways.
 * https://oeis.org/A345556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) of OEIS A345556,
   i.e., the nth positive integer that can be expressed
   as the sum of ten (positive) cubes in at least eight
   distinct ways.  The implementation uses a DP-sieve
   over sums of cubes and counts combinations
   (unordered) of exactly 10 cubes.  Results are memoized
   and the DP is recomputed only when more terms are
   needed.  The method is efficient enough for n up to
   about 10 000 well within the required time limit. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static data for memoization */
    static seq_t *terms  = NULL;
    static size_t  tcnt  = 0;      /* number of terms stored */
    static size_t  tcap  = 0;      /* allocated capacity */
    static size_t  lastS = 0;      /* largest sum processed so far */
    static size_t  maxS  = 200000; /* current upper bound for sums */

    /* Grow the table until we have at least n terms */
    while (tcnt < (size_t)n) {
        /* --- DP over sums of cubes up to maxS --- */
        size_t S = maxS;

        /* compute integer cube root limit M such that M^3 <= S */
        size_t M = 0;
        while ((M + 1) * (M + 1) * (M + 1) <= S) ++M;

        /* allocate DP array: dp[t][s] for t=0..10, s=0..S */
        size_t dp_rows     = 11;                          /* 0..10 */
        size_t dp_row_size = S + 1;                       /* sums */
        size_t dp_tot      = dp_rows * dp_row_size;       /* total cells */

        uint8_t *dp = (uint8_t *)calloc(dp_tot, 1);       /* zeroed */
        if (!dp) return 0; /* allocation failure guard */

        dp[0] = 1; /* dp[0][0] = 1 */

        /* iterate over cube values v = k^3 */
        for (size_t k = 1; k <= M; ++k) {
            size_t v = k * k * k; /* cube value */
            for (int t = 1; t <= 10; ++t) {
                size_t base_t  = t  * dp_row_size;
                size_t base_t1 = (t-1) * dp_row_size;
                for (size_t s = v; s <= S; ++s) {
                    uint8_t prev = dp[base_t1 + (s - v)];
                    if (prev) {
                        uint8_t cur = dp[base_t + s] + prev;
                        if (cur > 8) cur = 8; /* cap at 8 */
                        dp[base_t + s] = cur;
                    }
                }
            }
        }

        /* collect new terms with at least 8 representations */
        size_t start = lastS + 1;
        if (start < 10) start = 10; /* smallest possible sum with 10 cubes of 1 */
        for (size_t val = start; val <= S; ++val) {
            if (dp[10 * dp_row_size + val] >= 8) {
                if (tcnt == tcap) {
                    tcap = tcap ? tcap * 2 : 64;
                    terms = (seq_t *)realloc(terms, tcap * sizeof(seq_t));
                }
                terms[tcnt++] = (seq_t)val;
            }
        }

        free(dp);
        lastS = S;              /* update processed range */

        /* If not enough terms yet, enlarge the search space */
        if (tcnt < (size_t)n)
            maxS *= 2;          /* double the limit and recompute */
    }

    return terms[n - 1];
}
