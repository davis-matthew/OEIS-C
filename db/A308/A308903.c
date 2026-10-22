/*
 * A308903  Sum of all the parts in the partitions of n into 6 squarefree
 *          parts.
 * https://oeis.org/A308903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A308902(n) where A308902(n) is the number of
 *            partitions of n into exactly 6 squarefree parts
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies from the utils library */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* For n < 6 the answer is 0 */
    if (n < 6) return (seq_t)0;

    /* ----------- 1.  Determine square‑free status up to n -------------
       We use the classical sieve: mark all multiples of p² as non‑squarefree
       for each prime p ≤ √n.  */
    bool *sqfree = (bool *)malloc((n + 1) * sizeof(bool));
    if (!sqfree) return (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) sqfree[i] = true;
    sqfree[0] = true;          /* 0 is considered square‑free for the sieve */
    sqfree[1] = true;          /* 1 is square‑free */

    idx_t limit = (idx_t)int_sqrt(n);
    for (idx_t p = 2; p <= limit; ++p) {
        if (!sqfree[p]) continue;   /* composite, skip */
        idx_t sq = p * p;
        for (idx_t k = sq; k <= n; k += sq)
            sqfree[k] = false;
    }

    /* ----------- 2.  DP: count partitions of n into 6 square‑free parts ----------
       dp[k][s] – number of ways to write s as a sum of k non‑decreasing square‑free
       parts.  We only need k = 0 … 6.
       The recurrence:
       dp[0][0] = 1
       dp[k][s] += dp[k‑1][s‑v] for every square‑free part v ≤ s.           */

    seq_t **dp = (seq_t **)calloc(7, sizeof(seq_t *));
    if (!dp) { free(sqfree); return (seq_t)0; }
    for (idx_t k = 0; k <= 6; ++k) {
        dp[k] = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (!dp[k]) { 
            for (idx_t j = 0; j < k; ++j) free(dp[j]); 
            free(dp); free(sqfree); 
            return (seq_t)0; 
        }
    }
    dp[0][0] = (seq_t)1;   /* one way to obtain sum 0 with 0 parts */

    /* Main DP loop */
    for (idx_t v = 1; v <= n; ++v) {
        if (!sqfree[v]) continue;   /* skip non‑squarefree parts */
        for (idx_t k = 1; k <= 6; ++k) {
            seq_t *dp_k   = dp[k];
            seq_t *dp_km1 = dp[k - 1];
            for (idx_t s = v; s <= n; ++s)
                dp_k[s] += dp_km1[s - v];
        }
    }

    /* ----------- 3.  Result ------------------------------------------ */
    seq_t result = (seq_t)n * dp[6][n];

    /* ----------- 4.  Clean up -------------------------------------- */
    for (idx_t k = 0; k <= 6; ++k) free(dp[k]);
    free(dp);
    free(sqfree);

    return result;
}
