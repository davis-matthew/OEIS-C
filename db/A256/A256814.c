/*
 * A256814  Number of length n+6 0..1 arrays with at most two downsteps in
 *          every 6 consecutive neighbor pairs.
 * https://oeis.org/A256814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 27, u64 n <= 61, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A256814.
   The sequence counts binary strings of length n+6 in which any 6
   consecutive neighbor pairs contain at most two downsteps (a 1→0
   transition).  This is evaluated by dynamic programming over a 6‑bit
   state that keeps the last up to 6 bits of the string and the number
   of 1→0 transitions inside the current window. */

#include <stddef.h>
#include <stdbool.h>

/* Precomputed number of downsteps in each 6‑bit mask. */
static int inside_dows[64];
static bool dows_computed = false;

/* Fill the inside_dows table once. */
static void compute_inside_dows(void)
{
    if (dows_computed) return;
    for (int mask = 0; mask < 64; ++mask) {
        int cnt = 0;
        /* Count 1→0 transitions between bits k and k-1 (k = 1..5). */
        for (int k = 1; k <= 5; ++k) {
            int high = (mask >> k) & 1;
            int low  = (mask >> (k - 1)) & 1;
            if (high == 1 && low == 0) cnt++;
        }
        inside_dows[mask] = cnt;
    }
    dows_computed = true;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n = 0, return 0 */

    compute_inside_dows();

    idx_t L = n + 6;            /* required string length */

    seq_t dp[64]  = {0};        /* counts for current length */
    seq_t dp_next[64];

    dp[0] = (seq_t)1;           /* empty string */

    for (idx_t len = 1; len <= L; ++len) {
        for (int i = 0; i < 64; ++i) dp_next[i] = (seq_t)0;

        for (int mask = 0; mask < 64; ++mask) {
            seq_t cur = dp[mask];
            if (cur == (seq_t)0) continue;

            int dow = inside_dows[mask];
            int last = mask & 1;

            for (int b = 0; b <= 1; ++b) {
                int trans = (last == 1 && b == 0) ? 1 : 0;
                int total = dow + trans;
                if (total <= 2) {
                    int newmask = ((mask << 1) & 0x3F) | b;
                    dp_next[newmask] = dp_next[newmask] + cur;
                }
            }
        }
        /* prepare for next iteration */
        for (int i = 0; i < 64; ++i) dp[i] = dp_next[i];
    }

    /* Sum over all possible final states. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 64; ++i) {
        result = result + dp[i];
    }
    return result;
}
