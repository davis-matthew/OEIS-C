/*
 * A281050  Number of n X 2 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 30*a(n-2) + 45*a(n-3) - 30*a(n-4) + 9*a(n-5) -
 *            a(n-6) for n ≥ 7 with initial values: a(1)=0, a(2)=1, a(3)=6,
 *            a(4)=29, a(5)=122, a(6)=468
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* undefined, but safe */

    static const seq_t base[7] = {0, 0, 1, 6, 29, 122, 468};

    if (n <= 6) return base[n];

    seq_t f[6];
    /* initialise f with a(1)..a(6) */
    for (idx_t i = 0; i < 6; ++i)
        f[i] = base[i + 1];

    for (idx_t k = 7; k <= n; ++k) {
        /* compute a(k) from the last six values */
        seq_t next =
            (seq_t)9 * f[5]
          - (seq_t)30 * f[4]
          + (seq_t)45 * f[3]
          - (seq_t)30 * f[2]
          + (seq_t)9  * f[1]
          -          f[0];
        /* shift the window: drop the oldest and add new */
        for (int t = 0; t < 5; ++t)
            f[t] = f[t + 1];
        f[5] = next;
    }
    return f[5];
}
