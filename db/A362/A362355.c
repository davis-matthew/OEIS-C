/*
 * A362355  a(n) = 4*(n+4)^(n-1).
 * https://oeis.org/A362355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n+4)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) {
        return (seq_t)1;
    }

    /* base = n + 4  (seq_t) */
    seq_t base = (seq_t)n + (seq_t)4;
    /* exponent = n - 1  (idx_t) */
    idx_t exp = n - 1;
    /* result = 1  (seq_t) */
    seq_t result = (seq_t)1;

    /* fast exponentiation: result = base^exp */
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    /* multiply by 4 to get the final term */
    return result * (seq_t)4;
}
