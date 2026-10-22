/*
 * A334299  Number of distinct subsequences (not necessarily contiguous) of
 *          compositions in standard order (A066099).
 * https://oeis.org/A334299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct subsequences (including empty) of the
 *            composition corresponding to n in the standard order (A066099).
 *            The k‑th composition is obtained from n by: • list the positions
 *            (1‑based) of 1‑bits in the binary representation of n
 *            (traversing from LSB to MSB); • prepend 0, take fi...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find positions of bits set in n (LSB is position 1) */
    int positions[65];
    int pos_cnt = 0;
    idx_t temp = n;
    for (int i = 0; temp; ++i, temp >>= 1) {
        if (temp & 1)
            positions[pos_cnt++] = i + 1;   /* positions are 1‑based */
    }

    /* Build the composition parts: differences of consecutive positions,
       then reverse them           */
    int parts[65];
    int part_cnt = 0;
    int prev = 0;
    for (int i = 0; i < pos_cnt; ++i) {
        int diff = positions[i] - prev;
        parts[part_cnt++] = diff;   /* will reverse later */
        prev = positions[i];
    }

    /* Reverse the differences to obtain the composition */
    for (int i = 0; i < part_cnt / 2; ++i) {
        int t = parts[i];
        parts[i] = parts[part_cnt - 1 - i];
        parts[part_cnt - 1 - i] = t;
    }

    /* DP to count distinct subsequences (including empty) */
    seq_t dp[65];
    dp[0] = 1;                     /* empty subsequence */
    int last[65] = {0};             /* last occurrence index (1‑based) */

    for (int i = 1; i <= part_cnt; ++i) {
        seq_t prev = dp[i - 1];
        seq_t newdp = prev * 2;    /* double all previous subsequences */

        int part = parts[i - 1];   /* current symbol */
        int last_pos = last[part];
        if (last_pos) {            /* subtract subsequences that would repeat */
            newdp -= dp[last_pos - 1];
        }

        dp[i] = newdp;
        last[part] = i;            /* update last occurrence */
    }

    return dp[part_cnt];
}
