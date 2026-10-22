/*
 * A324648  a(n) = n - A318458(n), where A318458(n) is bitwise-AND of n and
 *          the sum of proper divisors of n (sigma(n)-n).
 * https://oeis.org/A324648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n - (n & (sigma(n)-n))
 * Range:     u8 n <= 255, u16 n <= 9822, u32 n <= 9822, u64 n <= 9822,
 *            u128 n <= 9822, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sigma(n) via divisor enumeration. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute sigma(n), the sum of all divisors of n. */
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i) {
                sigma += (seq_t)j;
            }
        }
    }

    seq_t proper = sigma - (seq_t)n;          /* sum of proper divisors */
    seq_t bit_and = (seq_t)n & proper;        /* n AND proper divisor sum */
    return (seq_t)n - bit_and;                /* a(n) */
}
