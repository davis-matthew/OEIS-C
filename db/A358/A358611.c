/*
 * A358611  a(n) = Sum_{k=0..floor(n/5)} (-1)^k * (n-5*k)!.
 * https://oeis.org/A358611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - a(n-5) + n*a(n-6) for n>5 Initial values (via
 *            the sum definition): a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=24,
 *            a(5)=119
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Buffer to store the last up to seven values a(i) for i modulo 7. */
    seq_t buf[7];

    /* Compute the base cases 0 .. 5 using the sum of factorials. */
    for (idx_t i = 0; i <= 5 && i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i / 5; ++k) {
            idx_t m = i - 5 * k;
            seq_t term = factorial(m);
            if (k & 1)
                sum -= term;
            else
                sum += term;
        }
        buf[i] = sum;
    }
    if (n <= 5)
        return buf[n];

    /* Compute remaining values using the recurrence relation. */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t part1   = (seq_t)i * buf[(i - 1) % 7];
        seq_t part3   = (seq_t)i * buf[(i - 6) % 7];
        seq_t a_minus5 = buf[(i - 5) % 7];
        buf[i % 7] = part1 - a_minus5 + part3;
    }

    return buf[n % 7];
}
