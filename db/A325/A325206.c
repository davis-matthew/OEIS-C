/*
 * A325206  a(n) is the number of labeled rooted trees on a set of size n
 *          where each node has at most 7 neighbors that are further away from
 *          the root than the node itself.
 * https://oeis.org/A325206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)! * [x^(n-1)] e_7(x)^n, e_7(x)=∑_{k=0}^7 x^k/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  DP over the n boxes (nodes).  For each box we keep
 *  dp[c] = number of ways to assign c nodes to the
 *  previously processed boxes using the truncated
 *  exponential e_7.  After processing all n boxes
 *  dp[n-1] is the coefficient of x^(n-1) in e_7(x)^n.   */

/*  The algorithm is O(n^2) in the number of boxes but with
 *  a small constant (at most 8 children per box).  It
 *  works for arbitrary-precision seq_t and for 64‑bit
 *  builds (the intermediate binomial coefficients are
 *  tiny enough for the 64‑bit range).                      */

#include <stdlib.h>

/*  Static arrays to reuse the memory between calls.  */
static idx_t dp_cap = 0;          /* capacity of dp and ndp arrays      */
static seq_t *dp    = NULL;       /* current DP state                  */
static seq_t *ndp   = NULL;       /* temporary DP state                */

/*  Ensure the DP arrays can hold (len) elements.  */
static void ensure_dp_cap(idx_t len)
{
    if (dp_cap < len) {
        idx_t new_cap = dp_cap > 0 ? dp_cap : 8;
        while (new_cap < len) new_cap <<= 1;
        dp  = (seq_t *)realloc(dp,  new_cap * sizeof(seq_t));
        ndp = (seq_t *)realloc(ndp, new_cap * sizeof(seq_t));
        dp_cap = new_cap;
    }
}

/*  The main sequence function.  */
seq_t a(idx_t n)
{
    /*  a(0) is defined as 0.  */
    if (n == 0) return (seq_t)0;

    /*  Remaining nodes to be distributed among the n boxes: m = n-1.  */
    idx_t m = n - 1;

    /*  Allocate or grow the DP arrays.  */
    ensure_dp_cap(m + 1);
    for (idx_t i = 0; i <= m; ++i) dp[i] = 0;
    dp[0] = 1;      /* base case: zero boxes processed, zero nodes assigned. */

    /*  Process each of the n boxes (nodes).  */
    for (idx_t box = 1; box <= n; ++box) {
        /*  Zero the temporary array.  */
        for (idx_t i = 0; i <= m; ++i) ndp[i] = 0;

        /*  Iterate over all currently assigned node counts.  */
        for (idx_t c = 0; c <= m; ++c) {
            seq_t val = dp[c];
            if (val == 0) continue;      /* skip zero contributions */

            idx_t remain = m - c;                         /* nodes yet to be assigned */
            int   max_d  = (remain < 7) ? (int)remain : 7;/* maximum children for this box */

            /*  Compute binomial coefficients C(remain,d) on the fly.  */
            seq_t binom = (seq_t)1;                       /* C(remain,0) */
            for (int d = 0; d <= max_d; ++d) {
                /*  Contribution for 'd' children */
                seq_t add = val * binom;
                ndp[c + d] += add;

                /*  Update binomial for next d, unless we are at max_d.  */
                if (d != max_d) {
                    /*  binom = binom * (remain - d) / (d + 1)  */
                    seq_t numer = binom * (seq_t)(remain - d);
                    binom = numer / (seq_t)(d + 1);
                }
            }
        }

        /*  Swap dp and ndp for the next box.  */
        seq_t *tmp = dp;
        dp        = ndp;
        ndp       = tmp;
    }

    /*  Result is the number of ways to assign all m nodes.  */
    return dp[m];
}
