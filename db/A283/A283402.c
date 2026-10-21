/*
 * A283402  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 633", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) defined via generating function G(x) =
 *            P(x)/((x-1)(2x-1)(2x+1)) and satisfies the recurrence a(n) =
 *            a(n-1) + 4*a(n-2) - 4*a(n-3) + p(n) for n ≥ 3, where p(n) are
 *            the coefficients of the polynomial P(x) = -12*x^26 + 8*x^25 +
 *            3*x^24 - 2*x^23 - 52*x^12 + 40*x^11 + 17*x^10 - 2*x^9 - x^8 -
 *            2*x^7...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values from the sequence definition */
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 3;

    /* p[n] for n = 0 .. 26, 0 otherwise */
    static const int32_t P[27] = {
        1,  -1,  -1,  2,  1,   0,   4,  -2,  -1,  -2,
        17, 40, -52,  0,  0,   0,   0,   0,   0,   0,
        0,   0,   0,  -2,  3,   8,  -12
    };

    seq_t prev3 = 1;   /* a(n-3)   */
    seq_t prev2 = 0;   /* a(n-2)   */
    seq_t prev1 = 3;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t pn = (i <= 26) ? (seq_t)P[i] : 0;
        /* a(i) = a(i-1) + 4*a(i-2) - 4*a(i-3) + p(i) */
        cur = prev1 + (prev2 << 2) - (prev3 << 2) + pn;

        /* shift window for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
