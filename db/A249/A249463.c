/*
 * A249463  Number of length n+4 0..5 arrays with no five consecutive terms
 *          having four times any element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 22, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249463: dynamic‑programming computation over the last four digits.
    For each n ≥ 1 it returns the number of length n+4 arrays over 0..5
    with no five consecutive terms satisfying the “four times any
    element = sum of the other four” condition. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static tables for base‑6 representation of 4‑digit states */
    static bool inited = false;
    static int sb0[1296], sb1[1296], sb2[1296], sb3[1296];

    /* DP buffers: dp_prev holds counts for current length, dp_cur for next
       (each index 0…1295 represents a 4‑digit state in base 6). */
    static seq_t dp0[1296], dp1[1296];
    static seq_t *dp_prev = dp0;
    static seq_t *dp_cur  = dp1;

    if (!inited) {
        for (int i = 0; i < 1296; ++i) {
            int x = i;
            sb3[i] = x % 6; x /= 6;
            sb2[i] = x % 6; x /= 6;
            sb1[i] = x % 6; x /= 6;
            sb0[i] = x;                 /* most significant digit */
        }
        inited = true;
    }

    if (n < 1) return (seq_t)0;          /* sequence undefined for n<1 */

    idx_t target_len = n + 4;            /* total length of the arrays */

    /* initialise DP for length 4 (any 4‑tuple is allowed) */
    for (int i = 0; i < 1296; ++i)
        dp_prev[i] = (seq_t)1;

    idx_t cur_len = 4;
    while (cur_len < target_len) {
        /* zero next‑length buffer */
        for (int i = 0; i < 1296; ++i)
            dp_cur[i] = (seq_t)0;

        /* transition from current 4‑state to next by adding a new element */
        for (int i = 0; i < 1296; ++i) {
            seq_t cnt = dp_prev[i];
            if (cnt == (seq_t)0) continue;

            int b0 = sb0[i], b1 = sb1[i], b2 = sb2[i], b3 = sb3[i];
            int base_sum = b0 + b1 + b2 + b3;

            for (int x = 0; x <= 5; ++x) {
                int S = base_sum + x;
                if (5*b0 == S || 5*b1 == S || 5*b2 == S ||
                    5*b3 == S || 5*x   == S)
                    continue;               /* window invalid */

                int next = ((b1*6 + b2)*6 + b3)*6 + x;
                dp_cur[next] += cnt;
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
        ++cur_len;
    }

    /* sum all counts for the final length */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 1296; ++i)
        total += dp_prev[i];

    return total;
}
