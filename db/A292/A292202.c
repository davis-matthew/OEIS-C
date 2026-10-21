/*
 * A292202  The n-th iteration of A062028 starting with n.
 * https://oeis.org/A292202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = applying f(x) = x + digit_sum(x) exactly n times to n
 * Range:     u8 n <= 24, u16 n <= 3053, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    for (idx_t i = 0; i < n; ++i)
        res += digit_sum(res);
    return res;
}
