/*
 * A294887  Sum of nondeficient proper divisors of n.
 * https://oeis.org/A294887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A294936(d)*d
 * Range:     u8 n <= 179, u16 n <= 9294, u32 n <= 9294, u64 n <= 9294,
 *            u128 n <= 9294, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"

/* returns A294936(d) = 1 if d is nondeficient, else 0 */
static bool is_nondeficient(uint64_t d)
{
    seq_t s = sigma_of(d);                       /* sigma(d) */
    seq_t two_d = (seq_t)2 * (seq_t)d;           /* 2*d */
    return s >= two_d;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* a(1) = 0 */

    uint64_t un = n;
    seq_t sum = 0;

    for (uint64_t i = 1; i <= un / i; ++i) {
        if (un % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = un / i;

            if (d1 < un && is_nondeficient(d1))
                sum += (seq_t)d1;

            if (d2 != d1 && d2 < un && is_nondeficient(d2))
                sum += (seq_t)d2;
        }
    }
    return sum;
}
