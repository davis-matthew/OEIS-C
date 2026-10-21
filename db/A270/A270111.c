/*
 * A270111  Number of n X 1 0..7 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling seven
 *          exactly once.
 * https://oeis.org/A270111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * 7^(n-2) * (n-1) for n >= 2, a(1) = 0 * * The
 *            recurrence a(n) = 14*a(n-1) - 49*a(n-2) is also satisfied. * A
 *            simple closed-form calculation using exponentiation by squaring
 *            * gives an O(log n) algorithm that works with
 *            arbitrary-precision * seq_t types
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed, not strictly required */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;               /* a(1) = 0, values for n<1 are irrelevant */

    /* Compute 7^(n-2) with fast exponentiation */
    seq_t pow7 = 1;
    seq_t base  = 7;
    idx_t exp   = n - 2;
    while (exp > 0) {
        if (exp & 1)
            pow7 = pow7 * base;
        base = base * base;
        exp >>= 1;
    }

    /* a(n) = 8 * 7^(n-2) * (n-1) */
    seq_t result = 8;
    result = result * pow7;
    result = result * (seq_t)(n - 1);

    return result;
}
