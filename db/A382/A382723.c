/*
 * A382723  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 4.
 * https://oeis.org/A382723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A382723 – Number of entries in the n‑th row of Pascal's
 * triangle not divisible by 4.
 *
 * The algorithm uses Kummer's theorem: the exponent of 2 in
 * C(n,k) equals the number of carries when adding k and n−k in binary.
 * Thus C(n,k) is not divisible by 4 iff that carry count is 0 or 1.
 *
 * A DP over the binary digits of n counts the admissible k
 * while keeping track of at most one carry.  Complexity is
 * O(#bits(n)) and is easily fast enough for n up to 10 000.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0: only k = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Determine number of bits used by n */
    unsigned bits = 0;
    {
        idx_t tmp = n;
        while (tmp) { ++bits; tmp >>= 1; }
    }

    /* dp[carry_cnt][carry_in] – number of ways for processed bits */
    seq_t dp[2][2] = { {0} };
    dp[0][0] = 1;                 /* no carry yet, no incoming carry */

    for (unsigned i = 0; i < bits; ++i) {
        seq_t ndp[2][2] = { {0} };
        unsigned ni = (unsigned)((n >> i) & 1);

        for (int carry_cnt = 0; carry_cnt <= 1; ++carry_cnt) {
            for (int carry_in = 0; carry_in <= 1; ++carry_in) {
                seq_t cur = dp[carry_cnt][carry_in];
                if (cur == (seq_t)0) continue;

                for (int kbit = 0; kbit <= 1; ++kbit) {
                    for (int carry_out = 0; carry_out <= 1; ++carry_out) {
                        int lbit = (int)ni + 2 * carry_out - kbit - carry_in;
                        if (lbit == 0 || lbit == 1) {
                            int new_cnt = carry_cnt + carry_out;
                            if (new_cnt <= 1)
                                ndp[new_cnt][carry_out] += cur;
                        }
                    }
                }
            }
        }
        dp[0][0] = ndp[0][0];
        dp[0][1] = ndp[0][1];
        dp[1][0] = ndp[1][0];
        dp[1][1] = ndp[1][1];
    }

    /* After processing all bits, the final carry must be 0 */
    seq_t result = dp[0][0] + dp[1][0];
    return result;
}
