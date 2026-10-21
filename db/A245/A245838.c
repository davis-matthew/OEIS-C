/*
 * A245838  Arithmetic derivative of (3*n + 1), n >= 1, (A016777)'.
 * https://oeis.org/A245838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of 3*n+1. a(n) = (3*n + 1)'. The
 *            arithmetic derivative of m = p1^e1 * p2^e2 * … is ∑ e_i * (m /
 *            p_i)
 * Range:     u8 n <= 52, u16 n <= 4436, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 3*n+1 fits in 64 bits for any standard N. */
    uint64_t m = 3ULL * n + 1;
    seq_t     res = 0;

    uint64_t tmp = m;

    /* Trial division – fast enough for n ≤ 10⁴ (m ≤ 3·10⁴+1). */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            /* term = exp * (m / p) */
            uint64_t term64 = (m / p) * exp;
            res += (seq_t)term64;
        }
    }

    /* Remaining prime factor, if any. */
    if (tmp > 1) {
        uint64_t term64 = m / tmp;   /* exp = 1 */
        res += (seq_t)term64;
    }

    return res;
}
