/*
 * A329023  Number of length-n ternary words having at most 5 palindromic
 *          subwords (including the empty word).
 * https://oeis.org/A329023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) for n ≥ 9 Initial values: a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=27, a(4)=81, a(5)=42, a(6)=54, a(7)=66, a(8)=78
 * Range:     u8 n <= 13, u16 n <= 41, u32 n <= 97, u64 n <= 208,
 *            u128 n <= 430, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

/* Return the n‑th term of sequence A329023.
   The function is re‑entrant; it does not rely on any external state
   and works for any idx_t/seq_t combination. */
seq_t a(idx_t n)
{
    /* Base values for n < 9.  Use an array indexed by size_t to avoid
       the warning about implicit cast of idx_t to size_t. */
    static const seq_t base[9] =
        {1, 3, 9, 27, 81, 42, 54, 66, 78};

    if (n < 9) {
        return base[(size_t)n];
    }

    /* We need only the last four values to compute subsequent terms. */
    seq_t buf[4];

    /* Initialise buf with a(5), a(6), a(7), a(8) – the last four terms
       before the recurrence starts. */
    buf[0] = base[5];   /* a(n-4) when n==9 */
    buf[1] = base[6];   /* a(n-3) when n==9 */
    buf[2] = base[7];
    buf[3] = base[8];   /* a(n-1) when n==9 */

    /* Iterate from 9 up to n, updating the sliding window. */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t val = buf[0] + buf[1];   /* a(i-4) + a(i-3) = a(i) */

        /* Shift the window one step forward: */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = val;
    }

    /* After the loop, buf[3] holds a(n). */
    return buf[3];
}
