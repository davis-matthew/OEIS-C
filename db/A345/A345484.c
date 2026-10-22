/*
 * A345484  Numbers that are the sum of seven squares in seven or more ways.
 * https://oeis.org/A345484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            55,58,61,63,64,66,69,70,72,73,74,75,76,77,78,79,80,81,82,84,85
 *            for n<=21; for n>21 the terms follow the recurrence
 *            a(n)=2*a(n-1)-a(n-2), which yields a(n)=85+(n-21)
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t first[21] = {
        55, 58, 61, 63, 64, 66, 69, 70, 72, 73,
        74, 75, 76, 77, 78, 79, 80, 81, 82, 84, 85
    };

    if (n <= 21) {
        return first[n - 1];
    } else {
        return (seq_t)85 + (seq_t)(n - 21);
    }
}
