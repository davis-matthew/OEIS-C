/*
 * A253229  Number of (n+1) X (6+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minimum minus antidiagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal maximum
 *          nondecreasing vertically.
 * https://oeis.org/A253229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1128 + 2451*2^(n-5) + 402*n + 33*n^2 for n>4 with initial
 *            values a(1)=2470, a(2)=4158, a(3)=3241, a(4)=4490 Source: OEIS
 *            A253229
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t init[4] = {
        (seq_t)2470,
        (seq_t)4158,
        (seq_t)3241,
        (seq_t)4490
    };

    if (n <= 4) {
        return init[n - 1];
    }

    seq_t pow = pow2(n - 5);
    seq_t result = (seq_t)2451 * pow;
    result += (seq_t)402 * n;
    seq_t n_sq = (seq_t)n * (seq_t)n;
    result += (seq_t)33 * n_sq;
    result += (seq_t)1128;
    return result;
}
