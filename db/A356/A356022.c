/*
 * A356022  Decimal expansion of the infinite series Sum_{j>=0} 1/2^^j where
 *          ^^ indicates tetration.
 * https://oeis.org/A356022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of the infinite series Sum_{j>=0}
 *            1/2^^j
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The series Σ_{j>=0} 1/2^^j converges to the rational 118785/65536. */
    static const uint64_t NUM = 118785ULL;
    static const uint64_t DEN = 65536ULL;

    if (n == 0) return 0;               /* undefined for n<1, return 0 for safety */
    if (n == 1) return (seq_t)(NUM / DEN);   /* the integer part */

    uint64_t rem = NUM % DEN;
    for (idx_t pos = 2; pos <= n; ++pos) {
        rem *= 10ULL;
        uint64_t d = rem / DEN;
        rem %= DEN;
        if (pos == n) return (seq_t)d;
    }
    return 0;   /* unreachable */
}
