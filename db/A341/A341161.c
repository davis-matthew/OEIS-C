/*
 * A341161  The sum of the prime digits of m and the sum of the nonprime
 *          digits of m differ by n.
 * https://oeis.org/A341161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the lexicographically earliest positive integer whose
   sum of prime digits and sum of non‑prime digits differ by n.
   For n ≤ 7 the smallest such integers are simply 1…7.
   For n > 7 all prime digits are absent (the difference is the sum
   of all digits) and the smallest integer with a given digit sum
   using only non‑prime digits {0,1,4,6,8,9} is constructed.
   The construction uses the minimal number of digits, computed by a
   simple DP over sums. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 7) return (seq_t)n;

    /* dynamic programming table: dp[s] = minimal number of non‑prime digits
       (from the set {1,4,6,8,9}) needed to obtain sum s. 0 is allowed
       only to pad remaining positions after the first digit. */
    static int *dp = NULL;
    static size_t dp_size = 0;

    size_t need = (size_t)n + 1;
    if (dp_size < need) {
        size_t new_size = dp_size * 2;
        if (new_size < need) new_size = need;
        int *tmp = (int *)realloc(dp, new_size * sizeof(int));
        if (!tmp) return 0;                    /* allocation failed */
        dp = tmp;
        /* initialise new entries to a large value */
        for (size_t i = dp_size; i < new_size; ++i) dp[i] = (int)(new_size + 5);
        dp[0] = 0;
        dp_size = new_size;
    }

    /* compute dp up to n */
    for (size_t s = 1; s <= n; ++s) {
        int best = dp[s];                       /* previously stored value */
        /* try each allowed non‑prime digit */
        if (s >= 1 && dp[s-1] + 1 < best) best = dp[s-1] + 1;
        if (s >= 4 && dp[s-4] + 1 < best) best = dp[s-4] + 1;
        if (s >= 6 && dp[s-6] + 1 < best) best = dp[s-6] + 1;
        if (s >= 8 && dp[s-8] + 1 < best) best = dp[s-8] + 1;
        if (s >= 9 && dp[s-9] + 1 < best) best = dp[s-9] + 1;
        dp[s] = best;
    }

    int digits_needed = dp[n];
    seq_t value = 0;
    int remaining_sum = (int)n;

    /* list of allowed digits, in increasing order */
    const int digs[6] = {0, 1, 4, 6, 8, 9};

    for (int pos = 0; pos < digits_needed; ++pos) {
        int remaining_positions = digits_needed - pos - 1;
        int start_idx = (pos == 0) ? 1 : 0;  /* first digit cannot be 0 */

        for (int idx = start_idx; idx < 6; ++idx) {
            int d = digs[idx];
            if (d > remaining_sum) break;
            int r = remaining_sum - d;
            if (dp[r] <= remaining_positions) {          /* feasible */
                value = value * (seq_t)10 + (seq_t)d;
                remaining_sum = r;
                break;
            }
        }
    }
    return value;
}
