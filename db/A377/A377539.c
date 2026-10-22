/*
 * A377539  The number of iterations of the map x -> x + A000005(x), starting
 *          from n, until reaching an even number, and always at least one
 *          iteration taken.
 * https://oeis.org/A377539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of iterations of x -> x + d(x) (d = number of
 *            divisors), until the result is even, with at least one iteration
 * Range:     u8 n <= 554, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t divisor_count(seq_t x)
{
    seq_t cnt = 1;
    seq_t tmp = x;
    for (seq_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n=0 is outside the sequence */
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (1) {
        x += divisor_count(x);
        ++steps;
        if ((x & 1) == 0) break;   /* x is even */
    }
    return steps;
}
