/*
 * A279041  Expansion of Product_{k>=1} 1/(1 - x^(k*(3*k-2))).
 * https://oeis.org/A279041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into nonzero octagonal numbers
 *            (octagonal numbers: k*(3*k-2) for k >= 1). Implements the DP
 *            recurrence a(n) = sum_{p=octagonal <= n} a(n-p) with a(0)=1
 * Range:     u8 n <= 157, u16 n <= 580, u32 n <= 2622, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *dp = NULL;         /* DP array: dp[i] = a(i) */
    static idx_t max_n = 0;          /* highest n for which dp is valid      */
    static idx_t *parts = NULL;      /* array of octagonal parts already known */
    static size_t parts_cap = 0, parts_len = 0;
    static idx_t last_k = 0;         /* last k for which part was generated   */

    /* Initialise DP array for n==0 on first call */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (dp == NULL) return 0;   /* Allocation failure */
        dp[0] = 1;
        max_n = 0;
    }

    /* If we need values up to a larger n, grow the data structures */
    if (n > max_n) {
        /* Generate all new octagonal numbers that are <= n */
        while (1) {
            idx_t k = last_k + 1;
            idx_t val = k * (3 * k - 2);   /* k*(3k-2) */
            if (val > n) break;

            /* Ensure capacity for the part */
            if (parts_len + 1 > parts_cap) {
                size_t newcap = parts_cap == 0 ? 1 : parts_cap * 2;
                idx_t *tmp = (idx_t *)realloc(parts, newcap * sizeof(idx_t));
                if (tmp == NULL) { free(dp); return 0; }
                parts = tmp;
                parts_cap = newcap;
            }
            parts[parts_len++] = val;
            last_k = k;
        }

        /* Allocate DP array up to n */
        seq_t *newdp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (newdp == NULL) { free(dp); return 0; }
        dp = newdp;

        /* Initialise new entries to zero */
        for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

        /* Recompute the entire DP up to n using all known parts */
        for (size_t pi = 0; pi < parts_len; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }

        max_n = n;
    }

    /* For n <= max_n, the answer is already in dp */
    return dp[n];
}
