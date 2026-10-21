/*
 * A244837  Number of length 6 0..n arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of length‑6 arrays (a1,…,a6) with each ai in
 *            [0..n] such that, * for every k = 1…6 let Sk = a1+…+ak. Then *
 *            |Sk - k·n/2| ≤ 2·σk, * where σk = sqrt(k·Var) and Var =
 *            n(n+2)/12. * After squaring and simplifying the inequality
 *            becomes: * 3·(2·Sk – k·n)² ≤ 4·k·n·(n+2). * * The function...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 40, u64 n <= 207, u128 n <= 207,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: check whether a partial sum s at position k satisfies the
   two‑standard‑deviation bound for a given n. */
static inline bool allowed(idx_t k, idx_t s, idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow. */
    __int128 T = (__int128)2 * s - (__int128)k * n;
    __int128 left  = (__int128)3 * T * T;
    __int128 right = (__int128)4 * k * n * (n + 2);
    return left <= right;
}

/* Dynamic‑programming implementation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* dp_prev holds the number of ways to reach each partial sum
       after k-1 elements.  Initially for k=0 we have only sum 0. */
    size_t prevSize = 1;
    seq_t *dp_prev = (seq_t*)calloc(prevSize, sizeof(seq_t));
    if (!dp_prev) return (seq_t)0;           /* allocation failed – return 0 */
    dp_prev[0] = (seq_t)1;

    /* Iterate over the 6 positions. */
    for (idx_t k = 1; k <= 6; ++k)
    {
        size_t currSize = (size_t)k * (size_t)n + 1;

        seq_t *dp_curr = (seq_t*)calloc(currSize, sizeof(seq_t));
        if (!dp_curr) { free(dp_prev); return (seq_t)0; }

        /* Prefix sums of dp_prev to enable O(1) range queries. */
        seq_t *prefix = (seq_t*)calloc(prevSize, sizeof(seq_t));
        if (!prefix) { free(dp_prev); free(dp_curr); return (seq_t)0; }

        if (prevSize > 0) {
            prefix[0] = dp_prev[0];
            for (size_t i = 1; i < prevSize; ++i)
                prefix[i] = prefix[i - 1] + dp_prev[i];
        }

        /* Compute dp_curr for all possible sums s. */
        for (size_t s = 0; s < currSize; ++s)
        {
            if (!allowed(k, (idx_t)s, (idx_t)n)) {
                dp_curr[s] = (seq_t)0;
                continue;
            }

            size_t low  = (s > (size_t)n) ? (s - (size_t)n) : 0;
            size_t high = (s < prevSize)   ? s : (prevSize - 1);

            seq_t val = prefix[high];
            if (low > 0) val -= prefix[low - 1];
            dp_curr[s] = val;
        }

        /* Free the old arrays and move on. */
        free(dp_prev);
        free(prefix);
        dp_prev   = dp_curr;
        prevSize  = currSize;
    }

    /* Sum all counts after the last position. */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < prevSize; ++i)
        ans += dp_prev[i];

    free(dp_prev);
    return ans;
}
