/*
 * A303908  Expansion of 1/(2 + x - theta_2(sqrt(x))/(2*x^(1/8))), where
 *          theta_2() is the Jacobi theta function.
 * https://oeis.org/A303908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 114, u64 n <= 219,
 *            u128 n <= 431, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for OEIS sequence A303908: number of compositions of n
   into triangular numbers > 1.
   Recurrence:
        a(0) = 1
        a(n) = Σ a(n – t)   over all triangular numbers t = k*(k+1)/2,
                with t > 1 and t ≤ n
*/
/* ------------------------------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system. */

static seq_t *dp     = NULL;         /* dynamic programming array */
static idx_t dp_cap  = 0;            /* capacity of dp array */
static idx_t dp_last = 0;            /* last computed index (dp[dp_last] is ready) */

static idx_t *tri   = NULL;         /* list of triangular numbers >1 */
static idx_t tri_cap   = 0;          /* capacity of tri array  */
static idx_t tri_len   = 0;          /* number of triangular numbers stored */
static idx_t tri_next_k = 2;         /* next k to generate T_k */

/* Ensure dp has at least (n+1) elements; initialise new slots to 0. */
static void ensure_dp(idx_t n)
{
    if (dp_cap > n) return;
    idx_t new_cap = dp_cap ? dp_cap : 1;
    while (new_cap <= n) new_cap <<= 1;   /* double until sufficient */
    seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!new_dp) abort();                 /* out‑of‑memory */
    /* initialise the new entries to 0 */
    for (idx_t i = dp_cap; i < new_cap; ++i) new_dp[i] = 0;
    dp     = new_dp;
    dp_cap = new_cap;
}

/* Ensure the array of triangular numbers contains all T_k ≤ n. */
static void ensure_tri(idx_t n)
{
    for (;;)
    {
        idx_t k = tri_next_k;
        idx_t t = (k * (k + 1)) / 2;       /* T_k */
        if (t > n) break;
        if (tri_len == tri_cap)
        {
            idx_t new_cap = tri_cap ? tri_cap * 2 : 4;
            idx_t *new_tri = (idx_t *)realloc(tri, new_cap * sizeof(idx_t));
            if (!new_tri) abort();          /* out‑of‑memory */
            tri   = new_tri;
            tri_cap = new_cap;
        }
        tri[tri_len++] = t;
        ++tri_next_k;
    }
}

seq_t a(idx_t n)
{
    /* Initialise base case a(0) = 1 on first call. */
    if (dp_cap == 0)
    {
        ensure_dp(0);
        dp[0] = 1;
        dp_last = 0;
    }

    /* Make sure dp has space up to index n. */
    ensure_dp(n);

    /* Make sure we know all triangular numbers needed. */
    ensure_tri(n);

    /* Compute all values from dp_last+1 up to n. */
    for (idx_t i = dp_last + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t j = 0; j < tri_len; ++j)
        {
            idx_t t = tri[j];
            if (t > i) break;
            sum += dp[i - t];
        }
        dp[i] = sum;
    }
    dp_last = n;

    return dp[n];
}
