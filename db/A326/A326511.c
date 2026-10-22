/*
 * A326511  Number of (binary) max-heaps on n elements from the set {0,1}
 *          containing exactly ten 0's.
 * https://oeis.org/A326511
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 90, u64 n <= 772,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = number of binary max‑heaps on n elements from {0,1}
 *          containing exactly ten 0's.
 *   The value is defined for n ≥ 10.
 *
 * Approach:
 *   For every node v of the complete binary heap (indexed 1..n) we
 *   compute the size sz[v] of its subtree.
 *   In a max‑heap with keys 0 and 1, a node may be 0 only if all
 *   nodes in its subtree are 0.  Thus the positions of 0's form a
 *   union of disjoint subtrees.  We choose a set of antichain nodes
 *   (no one is ancestor of another); selecting such a node forces
 *   its whole subtree to be 0.  The number of zeros contributed by
 *   a chosen node is sz[v].
 *
 *   For each node we tabulate the number of ways to obtain a given
 *   number k of zeros in its subtree when the node itself is 1.
 *   After computing the child contributions we can also add the
 *   possibility that the child is 0, which contributes exactly
 *   sz[child] zeros.  Finally we sum the contributions from both
 *   children to fill dp[v][k].
 *
 *   The root may also be 0, which is possible only when n = 10.
 *   That case is handled explicitly by adding 1 to the answer
 *   when n == 10.
 */
#include <stdlib.h>

/* Number of zeros we are interested in */
#define TARGET_ZEROS   10
/* Size of DP tables: 0 .. TARGET_ZEROS inclusive */
#define DP_DIM   (TARGET_ZEROS + 1)

/* ------------------------------------------------------------------ */
/* Helper structs ----------------------------------------------------- */
typedef struct {
    seq_t v[DP_DIM];   /* dp[v][k] : ways in subtree v with v = 1 and k zeros */
} dp_t;

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < TARGET_ZEROS)
        return 0;

    /* Subtree sizes */
    idx_t *sz = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    if (!sz) return 0;                /* allocation failed */

    dp_t *dp = (dp_t *)malloc((n + 2) * sizeof(dp_t));
    if (!dp) { free(sz); return 0; }

    /* Compute subtree sizes in bottom‑up order */
    for (idx_t i = n; i >= 1; --i) {
        idx_t s = 1;                     /* the node itself */
        idx_t l = 2 * i;
        if (l <= n) s += sz[l];
        idx_t r = 2 * i + 1;
        if (r <= n) s += sz[r];
        sz[i] = s;
    }

    /* DP bottom‐up */
    for (idx_t i = n; i >= 1; --i) {
        /* Initialise dp[i] to zero */
        for (size_t k = 0; k < DP_DIM; ++k)
            dp[i].v[k] = 0;

        /* Child contribution arrays */
        seq_t leftWays[DP_DIM];
        seq_t rightWays[DP_DIM];

        /* --- Left child ------------------------------------------- */
        idx_t L = 2 * i;
        if (L <= n) {
            /* child is 1 */
            for (size_t k = 0; k < DP_DIM; ++k)
                leftWays[k] = dp[L].v[k];
            /* child is 0 */
            if (sz[L] < DP_DIM)
                leftWays[sz[L]] += 1;
        } else {
            leftWays[0] = 1;   /* empty child contributes zero zeros */
            for (size_t k = 1; k < DP_DIM; ++k)
                leftWays[k] = 0;
        }

        /* --- Right child ------------------------------------------ */
        idx_t R = 2 * i + 1;
        if (R <= n) {
            for (size_t k = 0; k < DP_DIM; ++k)
                rightWays[k] = dp[R].v[k];
            if (sz[R] < DP_DIM)
                rightWays[sz[R]] += 1;
        } else {
            rightWays[0] = 1;
            for (size_t k = 1; k < DP_DIM; ++k)
                rightWays[k] = 0;
        }

        /* Combine left and right to fill dp[i] */
        for (size_t k = 0; k < DP_DIM; ++k) {
            seq_t total = 0;
            for (size_t a = 0; a <= k; ++a) {
                total += leftWays[a] * rightWays[k - a];
            }
            dp[i].v[k] = total;
        }
    }

    /* Result: root is 1, plus root zero case if n == TARGET_ZEROS */
    seq_t ans = dp[1].v[TARGET_ZEROS];
    if (n == TARGET_ZEROS)
        ans += 1;        /* root zero contributes exactly n zeros */

    /* Clean up */
    free(dp);
    free(sz);

    return ans;
}
