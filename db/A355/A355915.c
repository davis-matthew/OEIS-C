/*
 * A355915  Number of ways to write n as a sum of numbers of the form 2^r *
 *          3^s, where r and s are >= 0, and no summand divides another.
 * https://oeis.org/A355915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 934, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_ui
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the number of ways to write n as a sum of numbers
   of the form 2^r · 3^s (r,s ≥ 0) such that no summand divides another.
   The routine performs a depth‑first search over increasing r,
   keeping track of the maximum allowed s for the next choice so that
   the selected elements are pairwise incomparable under divisibility. */

#include <stdint.h>
#include "../utils/pow2_ui.h"

/* Return ⌊log2(x)⌋, where x ≥ 1.  Assumes idx_t is an unsigned integer type. */
static inline int floor_log2_uint64(idx_t x)
{
    int r = 0;
    while (x > 1) {
        x >>= 1;
        ++r;
    }
    return r;
}

/* Global variables used by the recursive search. */
static seq_t g_target;
static int    g_maxR, g_maxS;
static seq_t  g_ans;

/* Recursive search: r is the current exponent of 2 being considered.
   last_s is the upper bound on the exponent of 3 for the next chosen
   summand (s must be < last_s to keep the elements incomparable).
   sum is the current total of selected summands. */
static void dfs(int r, int last_s, seq_t sum)
{
    if (sum > g_target) return;          /* exceeded target – prune */

    if (r > g_maxR) {                    /* all exponents processed     */
        if (sum == g_target) g_ans++;    /* valid representation         */
        return;
    }

    /* Option 1: skip this exponent r entirely. */
    dfs(r + 1, last_s, sum);

    /* Option 2: choose one summand 2^r·3^s with s < last_s. */
    int maxS_local = last_s - 1;
    if (maxS_local > g_maxS) maxS_local = g_maxS;

    seq_t base = pow2_ui(r);             /* 2^r  (guaranteed ≤ target  */

    seq_t w = base;                      /* current 2^r·3^s              */
    int s = 0;
    while (s <= maxS_local) {
        if (w > g_target) break;                 /* further s only increases w   */

        if (sum <= g_target - w)                /* sum + w ≤ target?            */
            dfs(r + 1, s, sum + w);

        /* Prepare next s: multiply by 3, but stop if it would overflow
           or exceed the target. */
        if (w > g_target / 3) break;             /* would overflow or exceed     */
        w *= 3;
        ++s;
    }
}

/* Public API required by the build system. */
seq_t a(idx_t n)
{
    /* n is the index (1‑based).  Convert it to the target sum. */
    g_target = (seq_t)n;

    if (n == 0) return 0;            /* safety: offset is 1, but guard  */

    /* Maximum exponent r such that 2^r ≤ n. */
    g_maxR = floor_log2_uint64(n);

    /* Maximum exponent s such that 3^s ≤ n. */
    g_maxS = 0;
    seq_t val = 1;
    while (val <= g_target / 3) {
        val *= 3;
        ++g_maxS;
    }

    g_ans = 0;
    dfs(0, g_maxS + 1, (seq_t)0);
    return g_ans;
}
