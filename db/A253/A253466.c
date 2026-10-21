/*
 * A253466  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal minimum unequal to its
 *          neighbors horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n>6 Initial terms: a(1)=162016,
 *            a(2)=8536, a(3)=5000, a(4)=7496, a(5)=8000, a(6)=12680
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static const seq_t init[6] = {
        (seq_t)162016,
        (seq_t)8536,
        (seq_t)5000,
        (seq_t)7496,
        (seq_t)8000,
        (seq_t)12680
    };

    /* Use a small circular buffer of size 7 (indices 0..6) to avoid dynamic allocation */
    seq_t buf[7] = {0};

    /* Load initial values */
    for (idx_t i = 1; i <= 6; ++i)
        buf[i % 7] = init[i - 1];

    /* Compute via recurrence */
    for (idx_t i = 7; i <= n; ++i)
        buf[i % 7] = 5 * buf[(i - 2) % 7] - 4 * buf[(i - 4) % 7];

    return buf[n % 7];
}
