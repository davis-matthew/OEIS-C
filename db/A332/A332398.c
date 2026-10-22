/*
 * A332398  Number of set partitions of [n] where all prime-indexed blocks are
 *          singletons.
 * https://oeis.org/A332398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based implementation for OEIS A332398:
   a(0) = 1.
   For n >= 1, let dp[i][B] be the number of partitions of [i] into B blocks
   satisfying the “prime‑indexed blocks are singletons” condition.
   Transition:
       add to an existing composite block     → dp[i][B]  += dp[i-1][B] * (B - pi(B))
       start a new block (index B+1)           → dp[i][B+1] += dp[i-1][B]
   Here pi(B) is the number of primes ≤ B.
   The answer is sum_B dp[n][B].  The code implements this DP in O(n²).  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* ---------- Pre‑compute primes up to n+1 ---------- */
    idx_t limit = n + 1;                     /* we need primes up to n+1 */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;          /* out of memory */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    size_t *pi = (size_t*)malloc((size_t)(limit + 1) * sizeof(size_t));
    if (!pi) { free(is_prime); return (seq_t)0; }
    pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi[i] = pi[i - 1] + (is_prime[i] ? 1 : 0);

    /* ---------- DP arrays ---------- */
    size_t maxB = limit + 1;                 /* enough for indices up to n+1 */
    seq_t *dp_prev = (seq_t*)calloc(maxB, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t*)calloc(maxB, sizeof(seq_t));
    if (!dp_prev || !dp_cur) {
        free(is_prime); free(pi);
        return (seq_t)0;
    }
    dp_prev[0] = (seq_t)1;                   /* dp[0][0] = 1 */

    /* ---------- Main DP loop ---------- */
    for (idx_t i = 1; i <= n; ++i) {
        size_t maxB_i = (size_t)i;           /* after i elements, blocks ≤ i */
        for (size_t b = 0; b <= maxB_i; ++b) dp_cur[b] = (seq_t)0;

        for (size_t b = 0; b <= (size_t)(i - 1); ++b) {
            seq_t val = dp_prev[b];
            if (val == (seq_t)0) continue;

            /* Add to an existing composite block */
            size_t composite = b - pi[b];
            if (composite > 0) {
                dp_cur[b] += val * (seq_t)composite;
            }

            /* Start a new block (index b+1) */
            dp_cur[b + 1] += val;
        }

        /* swap dp_prev and dp_cur for next iteration */
        seq_t *tmp = dp_prev; dp_prev = dp_cur; dp_cur = tmp;
    }

    /* ---------- Sum over all block counts ---------- */
    seq_t result = (seq_t)0;
    for (size_t b = 0; b <= (size_t)n; ++b)
        result += dp_prev[b];

    /* ---------- Clean up ---------- */
    free(is_prime);
    free(pi);
    free(dp_prev);
    free(dp_cur);

    return result;
}
