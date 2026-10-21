/*
 * A284829  Expansion of Sum_{i>=1} mu(i)^2*x^i/(1 - x^i) * Product_{j>=i}
 *          1/(1 - mu(j)^2*x^j), where mu() is the Moebius function (A008683).
 * https://oeis.org/A284829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   total number of smallest parts in all partitions of n into
 *            squarefree parts (A284829)
 * Range:     u8 n <= 13, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static workspace ----------------------------------------------------------- */
static seq_t *dp = NULL;          /* partitions using parts >= current part   */
static seq_t *ans = NULL;         /* resulting a(n)                           */
static uint64_t *sqfree = NULL;   /* list of squarefree numbers up to max_n   */
static size_t sqfree_cnt = 0;     /* number of squarefree numbers             */
static idx_t max_n = 0;           /* highest n computed so far                */
static bool need_compute = false; /* flag indicating that recomputation is needed */

/* Ensure workspace can hold indices up to n --------------------------------- */
static void ensure_size(idx_t n)
{
    if (n <= max_n) return;          /* already big enough */

    /* free old arrays */
    free(dp);
    free(ans);
    free(sqfree);

    /* allocate new arrays (size_t conversion safe for n up to 64‑bit) */
    dp   = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    ans  = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp || !ans) abort();        /* allocation failure */

    /* build list of squarefree numbers up to n */
    bool *is_sqfree = (bool*)calloc((size_t)n + 1, sizeof(bool));
    if (!is_sqfree) abort();
    for (idx_t i = 1; i <= n; ++i) is_sqfree[i] = true;
    for (idx_t p = 2; p * p <= n; ++p) {
        idx_t pp = p * p;
        for (idx_t m = pp; m <= n; m += pp) is_sqfree[m] = false;
    }
    /* count and store squarefree numbers */
    sqfree_cnt = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (is_sqfree[i]) ++sqfree_cnt;
    sqfree = (uint64_t*)malloc(sqfree_cnt * sizeof(uint64_t));
    if (!sqfree) abort();
    size_t k = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (is_sqfree[i]) sqfree[k++] = (uint64_t)i;
    free(is_sqfree);

    max_n = n;
    need_compute = true;            /* recompute after reallocation */
}

/* Compute all values up to current max_n ------------------------------------ */
static void compute_all(void)
{
    /* initialize dp[0] and clear others */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_n; ++i) dp[i] = (seq_t)0;

    /* clear answer array */
    for (idx_t i = 0; i <= max_n; ++i) ans[i] = (seq_t)0;

    /* process parts from largest to smallest */
    for (size_t si = sqfree_cnt; si > 0; ) {
        --si;
        idx_t s = (idx_t)sqfree[si];

        /* contribution of minimal part s */
        for (idx_t m = 0; m <= max_n; ++m) {
            seq_t dm = dp[m];
            if (dm == (seq_t)0) continue;
            idx_t n = m + s;
            idx_t t = 1;
            while (n <= max_n) {
                ans[n] = ans[n] + dm * (seq_t)t;
                ++t;
                n += s;
            }
        }

        /* update dp to include part s (partitions with parts >= s) */
        for (idx_t n = s; n <= max_n; ++n) {
            dp[n] = dp[n] + dp[n - s];
        }
    }

    need_compute = false;           /* values are up to date */
}

/* Public sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset starts at 1 */

    ensure_size(n);
    if (need_compute) compute_all();
    return ans[n];
}
