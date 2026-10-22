/*
 * A394017  Expansion of g.f.: Product_{k>=1} 1 / (1 - x^(k^2 + 2)).
 * https://oeis.org/A394017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 319, u32 n <= 1226, u64 n <= 5814,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the number of partitions of n into parts of the form k^2+2
   (with k ≥ 1).  The implementation uses a standard dynamic‑programming
   DP array that is grown lazily so that the function is efficient even when
   called with arbitrary indices in any order. */

/*--- System headers -------------------------------------------------------*/
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

/*--- Global DP state -----------------------------------------------------*/
static seq_t *dp = NULL;        /* DP array storing a(0)...a(max_n) */
static size_t dp_cap = 0;       /* number of elements currently allocated */

/*--- Sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Expand the DP table if necessary */
    if ((size_t)(n + 1) > dp_cap) {
        /* Free previous table if it exists */
        if (dp != NULL) {
            free(dp);
        }

        /* Allocate new table of size n+1 */
        dp = (seq_t *)malloc(sizeof(seq_t) * (size_t)(n + 1));
        dp_cap = (size_t)(n + 1);

        /* Initialise: only one way to partition 0 (empty partition) */
        dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i) {
            dp[i] = 0;
        }

        /* Dynamic programming over allowed part sizes k^2 + 2 */
        for (idx_t k = 1;; ++k) {
            seq_t part = (seq_t)k * (seq_t)k + 2;   /* part = k^2 + 2 */
            if (part > (seq_t)n) {                  /* no larger parts needed */
                break;
            }
            size_t step = (size_t)part;            /* safe cast: part ≤ n ≤ idx_t max */
            for (size_t j = step; j <= (size_t)n; ++j) {
                dp[j] += dp[j - step];             /* add ways using this part */
            }
        }
    }

    return dp[(size_t)n];
}
