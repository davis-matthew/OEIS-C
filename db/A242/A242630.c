/*
 * A242630  Number of n-length words w over a 7-ary alphabet {a_1,...,a_7}
 *          such that w contains never more than j consecutive letters a_j
 *          (for 1<=j<=7).
 * https://oeis.org/A242630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑length words w over a 7‑ary alphabet *
 *            {a_1,…,a_7} such that w contains never more than j consecutive *
 *            letters a_j (for 1 <= j <= 7)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    /* State (j,r) : letter j (1..7), current run length r (1..j) */
    static const int offset[8] = {0, 1, 3, 6, 10, 15, 21, 28};

    seq_t cur[28] = {0};
    seq_t next[28] = {0};

    /* Length 1: each letter starts a run of length 1 */
    for (int j = 1; j <= 7; ++j)
        cur[offset[j - 1]] = 1;

    /* For n == 1 we are already done; for larger n iterate */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* Zero the next array */
        for (int i = 0; i < 28; ++i) next[i] = 0;

        /* Transition from every current state */
        for (int j = 1; j <= 7; ++j)
        {
            int base = offset[j - 1];
            for (int r = 1; r <= j; ++r)
            {
                seq_t val = cur[base + (r - 1)];
                if (val == 0) continue;

                /* Stay with the same letter if allowed */
                if (r < j)
                    next[base + r] += val;   /* run length r+1 */

                /* Switch to any other letter, start run length 1 */
                for (int k = 1; k <= 7; ++k)
                {
                    if (k == j) continue;    /* cannot switch to the same letter */
                    next[offset[k - 1]] += val;
                }
            }
        }

        /* Prepare for the next iteration */
        for (int i = 0; i < 28; ++i) cur[i] = next[i];
    }

    /* Sum all ending states */
    seq_t total = 0;
    for (int i = 0; i < 28; ++i) total += cur[i];
    return total;
}
