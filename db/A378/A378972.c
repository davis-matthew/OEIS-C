/*
 * A378972  Second differences of the strict partition numbers A000009.
 * https://oeis.org/A378972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p_strict(n+2) - 2*p_strict(n+1) + p_strict(n)
 * Range:     i8 n <= 60, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p_strict(k) is the number of strict (distinct‑part) partitions of k. */

#include <stddef.h>
#include <stdlib.h>

/* Cache for strict partition numbers p_strict(0 .. dp_limit) */
static seq_t *dp = NULL;
static idx_t dp_limit = 0;

/* Compute p_strict(i) for 0 <= i <= up (inclusive) */
static void compute_dp(idx_t up)
{
    /* allocate new array */
    seq_t *newdp = (seq_t *)malloc((size_t)(up + 1) * sizeof(seq_t));
    if (!newdp)
        return;                      /* allocation failed – caller will return 0 */

    newdp[0] = 1;                    /* p_strict(0) = 1 */
    for (idx_t i = 1; i <= up; ++i)
        newdp[i] = 0;

    /* dynamic programming: classic distinct‑part recurrence */
    for (idx_t k = 1; k <= up; ++k)
        for (idx_t i = up; i >= k; --i)
            newdp[i] += newdp[i - k];

    /* replace old cache, freeing the previous memory */
    free(dp);
    dp = newdp;
    dp_limit = up;
}

/* Ensure dp contains values up to index up (inclusive) */
static void ensure_dp(idx_t up)
{
    if (dp_limit >= up)
        return;
    compute_dp(up);
}

/* Required sequence entry */
seq_t a(idx_t n)
{
    /* p_strict(n), p_strict(n+1), p_strict(n+2) are needed */
    idx_t up = n + 2;
    ensure_dp(up);

    seq_t res = dp[n + 2];
    res += dp[n];
    seq_t twice = dp[n + 1];
    twice += twice;          /* twice = 2 * p(n+1) */
    res -= twice;

    return res;
}
