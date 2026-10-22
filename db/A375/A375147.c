/*
 * A375147  a(1) = 0; for n >= 2, a(n) is the number of iterations needed for
 *          the map: x -> x / A000005(x) if x is divisible by A000005(x), x ->
 *          x + 1 otherwise, to reach 1.
 * https://oeis.org/A375147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations needed for the map x → x / d(x) if x
 *            is divisible by d(x), else x → x + 1, to reach 1, where d(x) =
 *            A000005(x), the number of divisors of x
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t divisor_count_uint64(uint64_t n) {
    uint64_t r = 1;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            r *= (e + 1);
        }
    }
    if (t > 1) r *= 2;
    return r;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t x = (uint64_t)n;
    uint64_t steps = 0;
    while (x > 1) {
        uint64_t d = divisor_count_uint64(x);
        if (x % d == 0) {
            x /= d;
        } else {
            ++x;
        }
        ++steps;
    }
    return (seq_t)steps;
}
