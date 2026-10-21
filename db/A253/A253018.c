/*
 * A253018  Number of (n+2) X (1+2) 0..3 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) for n>5; a(1)=25, a(2)=37, a(3)=51, a(4)=63,
 *            a(5)=105
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 52, u64 n <= 113, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..5 (0 reserved for out‑of‑range) */
    static const seq_t base[6] = {0, 25, 37, 51, 63, 105};

    if (n < 1)
        return 0;          /* undefined for n < 1, return 0 as a sentinel */

    /* reduce n to the range 1..5 by subtracting 3 repeatedly */
    idx_t m = n;
    while (m > 5)
        m -= 3;

    /* start with the base value and multiply by 3 for each step of 3 removed */
    seq_t val = base[m];
    idx_t steps = (n - m) / 3;

    for (idx_t i = 0; i < steps; ++i)
        val *= 3;

    return val;
}
