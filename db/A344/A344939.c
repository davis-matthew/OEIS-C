/*
 * A344939  The part of n left of the radix point when written in base Pi
 *          using a greedy algorithm representation.
 * https://oeis.org/A344939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer part of the base‑π greedy * representation
 *            of n. It is obtained by repeatedly * determining the largest
 *            digit d ∈ {0,1,2,3} such that * d·π^k ≤ n, subtracting d·π^k
 *            from the remainder, and * concatenating the digits in base‑10.
 *            The algorithm * uses only integer arithmetic for...
 * Range:     u8 n <= 29, u16 n <= 306, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

extern seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* π to sufficient precision for n ≤ 10 000 */
    static const double pi = 3.14159265358979323846264338327950288L;

    /* 1. Find the highest power of π that does not exceed n. */
    double pow_pi = 1.0L;
    idx_t k = 0;
    while (pow_pi * pi <= (double)n) {
        pow_pi *= pi;
        ++k;
    }

    /* 2. Greedy extraction of digits. */
    double remainder = (double)n;
    seq_t result = 0;
    for (idx_t i = k; ; --i) {
        /* digit = floor(remainder / pow_pi), add tiny epsilon
           to guard against floating‑point rounding bugs. */
        uint64_t digit = (uint64_t)((remainder / pow_pi) + 1e-12L);

        remainder -= digit * pow_pi;
        result = result * 10 + (seq_t)digit;

        if (i == 0u)
            break;
        pow_pi /= pi;
    }

    return result;
}
