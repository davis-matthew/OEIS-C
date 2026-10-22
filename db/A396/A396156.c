/*
 * A396156  a(n) is the number of possible outcomes having a run of 7 or more
 *          sixes when a die is rolled n times.
 * https://oeis.org/A396156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - S(n), where S(n) counts sequences of length n that
 *            contain NO run of 7 consecutive sixes. The DP maintains states
 *            0..6: number of consecutive sixes at the end of the current
 *            prefix, all transitions avoiding a 7th six
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t f[7] = {0}, g[7] = {0};
    f[0] = (seq_t)1;                     /* empty sequence has 0 consecutive sixes */

    for (idx_t i = 0; i < n; ++i) {
        for (int k = 0; k < 7; ++k) g[k] = (seq_t)0;
        for (int k = 0; k < 7; ++k) {
            seq_t val = f[k];
            if (val == (seq_t)0) continue;

            /* roll not six: 5 possibilities -> state 0 */
            g[0] += val * (seq_t)5;

            /* roll six: only allowed if k < 6 -> state k+1 */
            if (k < 6) g[k + 1] += val;
        }
        /* copy next state to current */
        for (int k = 0; k < 7; ++k) f[k] = g[k];
    }

    /* sum of states gives number of sequences without 7 consecutive sixes */
    seq_t S = (seq_t)0;
    for (int k = 0; k < 7; ++k) S += f[k];

    seq_t total = pow_uint((uint64_t)6, n);
    return total - S;
}
