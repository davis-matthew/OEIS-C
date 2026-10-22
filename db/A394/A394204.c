/*
 * A394204  Expansion of Product_{k>=1} (1 + x^(k*(2*k-1))) / (1 -
 *          x^(k*(2*k-1))).
 * https://oeis.org/A394204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 223, u32 n <= 1035, u64 n <= 5614,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A394204:   Coefficient of x^n in  
    ∏_{k≥1} (1 + x^{k(2k-1)}) / (1 - x^{k(2k-1)}).
    The product can be rewritten as  
    ∏_{k≥1} (1 + 2·x^{k(2k-1)} + 2·x^{2k(2k-1)} + … ) .
    A dynamic‑programming routine is used to evaluate the generating
    function up to the requested n. */

/*  Includes common headers required by the build.  */
#include <stdlib.h>      /*  malloc, calloc, free  */
#include <stdint.h>      /*  uint64_t  */
#include <stddef.h>      /*  size_t  */
#include <string.h>      /*  memset  */

#ifdef __cplusplus
#include <type_traits>
static constexpr bool IS_INTEGRAL = std::is_integral<seq_t>::value;
#endif

/*  Dynamically allocated tables that hold the DP values.  */
static seq_t *dp       = NULL;   /*  current DP table  */
static seq_t *dp_next  = NULL;   /*  temporary table   */
static size_t dp_size  = 0;      /*  number of entries (>= max n processed)  */

/*  Allocate space for the DP tables if the requested size is larger
    than the current allocation.  */
static void ensure_arrays(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (dp_size < need) {
        free(dp);
        free(dp_next);
        dp       = (seq_t *)calloc(need, sizeof(seq_t));
        dp_next  = (seq_t *)calloc(need, sizeof(seq_t));
        dp_size  = need;
    }
}

/*  Recompute the DP values up to index n.  */
static void compute_dp(idx_t n)
{
    ensure_arrays(n);

    /*  Initialise dp[0] = 1, others = 0.  */
    memset(dp, 0, dp_size * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /*  Iterate over all part sizes m = k(2k-1).  */
    for (idx_t k = 1;; ++k) {
        idx_t m = k * (2 * k - 1);
        if (m > n) break;

        /*  For each residue modulo m, compute contributions.  */
        for (idx_t r = 0; r < m; ++r) {
            seq_t sum = 0;   /*  Sum of dp[j] for earlier positions in this residue  */
            for (idx_t i = r; i <= n; i += m) {
#if defined(__cplusplus)
                if constexpr (IS_INTEGRAL) {
                    /*  Use 128‑bit arithmetic to avoid overflow for integral types.  */
                    unsigned __int128 tmp = (unsigned __int128)dp[i] +
                                            ((unsigned __int128)sum << 1);
                    dp_next[i] = (seq_t)tmp;
                } else {
                    /*  seq_t is an arbitrary‑precision type: use ordinary addition.  */
                    dp_next[i] = dp[i] + sum + sum;
                }
#else
                /*  For C, seq_t is assumed to be integral; __int128 is available.  */
                unsigned __int128 tmp = (unsigned __int128)dp[i] +
                                        ((unsigned __int128)sum << 1);
                dp_next[i] = (seq_t)tmp;
#endif
                sum += dp[i];
            }
        }

        /*  Prepare for the next part size.  */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }
}

/*  Public function required by the tests.  */
seq_t a(idx_t n)
{
    compute_dp(n);
    return dp[n];
}
