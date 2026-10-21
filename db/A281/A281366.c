/*
 * A281366  Lexicographically least strictly increasing sequence such that,
 *          for any n>0, Sum_{k=1..n} a(k) can be computed without carries in
 *          base 9 (the numbers are written in base 9).
 * https://oeis.org/A281366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10 * a(n-4) for n > 7 Initial values: a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=10, a(5)=11, a(6)=20, a(7)=21 This implements the
 *            lexicographically least strictly increasing sequence whose
 *            partial sums can be added without carries in base 9. The
 *            implementation uses the recurrence to compute any term in...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 39, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static const seq_t init[8] = {
    0,  /* unused, indices start at 1 */
    1, 2, 3, 10, 11, 20, 21
};

seq_t a(idx_t n)
{
    if (n <= 7)
        return init[n];

    idx_t m = n;
    idx_t t = 0;

    /* Reduce n by 4 until it falls into the initial block. */
    while (m > 7) {
        m -= 4;
        t += 1;
    }

    /* Combine the initial term with the appropriate power of 10. */
    seq_t result = init[m] * pow10(t);
    return result;
}
