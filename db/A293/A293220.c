/*
 * A293220  Sum of all partial fractions in the algorithm used for calculation
 *          of A053446(n).
 * https://oeis.org/A293220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the iterates of the "divide by 3 modulo m" process
 *            x -> (x + r*m)/3 (r in {0,1,2}, then strip factors 3), starting
 *            at x = 1 and stopping at the first return to 1, where m =
 *            A001651(n) = floor((3n-1)/2)
 * Range:     u8 n <= 19, u16 n <= 299, u32 n <= 13122, u64 n <= 13122,
 *            u128 n <= 13122, big any n
 * Validated: standard, pass, 86 terms through n = 9985 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (3ULL * (uint64_t)n - 1ULL) / 2ULL;
    uint64_t x = 1;
    seq_t sum = 0;
    do {
        unsigned r;
        if (x % 3 == 0) r = 0;
        else if ((x + m) % 3 == 0) r = 1;
        else r = 2;
        x = (x + (uint64_t)r * m) / 3;
        while (x % 3 == 0) x /= 3;
        sum += (seq_t)x;
    } while (x != 1);
    return sum;
}
