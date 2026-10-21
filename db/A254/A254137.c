/*
 * A254137  Indices of centered hexagonal numbers (A003215) which are also
 *          pentagonal numbers (A000326).
 * https://oeis.org/A254137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1154*a(n-2) - 1154*a(n-3) - a(n-4) + a(n-5)
 *            Initial terms: a(1)=1, a(2)=52, a(3)=629, a(4)=59432,
 *            a(5)=725289 Computes the nth term with a linear recurrence,
 *            using only a sliding window of the five most recent values. This
 *            works for any seq_t, including arbitrary‑precision types
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Initial five values (1-indexed) */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)52,
        (seq_t)629,
        (seq_t)59432,
        (seq_t)725289
    };

    if (n <= 5)
        return init[n - 1];

    /* Sliding window: a1..a5 correspond to a(k-5)..a(k-1) */
    seq_t a1 = init[0];   /* a(k-5) */
    seq_t a2 = init[1];   /* a(k-4) */
    seq_t a3 = init[2];   /* a(k-3) */
    seq_t a4 = init[3];   /* a(k-2) */
    seq_t a5 = init[4];   /* a(k-1) */

    seq_t new_val;
    for (idx_t k = 6; k <= n; ++k) {
        /* a(k) = a(k-1) + a(k-5) - a(k-4) + 1154*(a(k-2)-a(k-3)) */
        new_val = a5 + a1 - a2 + ((a4 - a3) * (seq_t)1154);

        /* Shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = new_val;
    }

    return a5;
}
