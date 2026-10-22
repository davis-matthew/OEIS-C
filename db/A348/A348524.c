/*
 * A348524  Number of compositions (ordered partitions) of n into two or more
 *          cubes.
 * https://oeis.org/A348524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 109, u64 n <= 215,
 *            u128 n <= 426, big any n
 * Helpers:   icube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n into two or more cubes.
   dp[0] = 1; dp[n] = Σ_{c³ ≤ n} dp[n - c³]  (order matters).
   a(n) = dp[n] - (n is a cube ? 1 : 0),  a(0) = 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/icube_root.h"

/* Memoised dynamic programming table. */
static seq_t *dp = NULL;          /* array of dp[0…max_comp] */
static size_t dp_cap = 0;         /* allocated capacity */
static idx_t max_comp = 0;        /* highest index computed so far */

/* Ensure dp[0…n] are defined. */
static void ensure_dp(idx_t n)
{
    if (n <= max_comp)
        return;

    /* Expand capacity if needed. */
    size_t need = (size_t)(n + 1);
    if (need > dp_cap) {
        seq_t *newdp = (seq_t *)realloc(dp, need * sizeof(seq_t));
        if (!newdp)
            abort();  /* allocation failed */
        /* Initialise new entries to zero. */
        if (dp_cap < need)
            for (size_t i = dp_cap; i < need; ++i)
                newdp[i] = (seq_t)0;
        dp = newdp;
        dp_cap = need;
    }

    /* Initialise dp[0] to 1 only on first allocation. */
    if (max_comp == 0)
        dp[0] = (seq_t)1;

    /* Compute dp values up to n. */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        idx_t r = icube_root(k);
        for (idx_t j = 1; j <= r; ++j) {
            idx_t cube = j * j * j;
            sum += dp[k - cube];
        }
        dp[k] = sum;
    }
    max_comp = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* by definition */

    ensure_dp(n);

    seq_t ans = dp[n];
    /* Subtract the single‑cube composition if n itself is a cube. */
    idx_t r = icube_root(n);
    if (r * r * r == n)
        ans -= (seq_t)1;

    return ans;
}
