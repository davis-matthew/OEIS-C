/*
 * A251991  Numbers n such that the sum of the pentagonal numbers P(n) and
 *          P(n+1) is equal to the sum of the hexagonal numbers H(m) and
 *          H(m+1) for some m.
 * https://oeis.org/A251991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 194*a(n-1) - a(n-2) + 64 Initial values: a(1) = 60, a(2)
 *            = 11704, a(3) = 2270580
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* for n < 1 the sequence is undefined – return 0 */
    if (n == 0)
        return (seq_t)0;

    /* initial terms */
    const seq_t a1 = (seq_t)60;
    const seq_t a2 = (seq_t)11704;
    const seq_t a3 = (seq_t)2270580;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* iteration variables: a(n-2) and a(n-1) */
    seq_t prev2 = a1; /* a(1) */
    seq_t prev1 = a2; /* a(2) */
    seq_t curr = a3;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        prev2 = prev1;
        prev1 = curr;
        /* recurrence: a(n) = 194*a(n-1) - a(n-2) + 64 */
        curr = (seq_t)194 * prev1 - prev2 + (seq_t)64;
    }

    return curr;
}
