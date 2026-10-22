/*
 * A383702  Coefficient of x^2 in expansion of (x+3) * (x+7) * ... *
 *          (x+4*n-1).
 * https://oeis.org/A383702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the coefficient of x^2 in (x+3)(x+7)...(x+4n-1) Computed
 *            via the recurrence: a(n) = 3*(4n-5)*a(n-1) -
 *            (48n^2-168n+151)*a(n-2) + (4n-9)^3*a(n-3) with a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;

    seq_t a0 = 0; /* a(n-3) */
    seq_t a1 = 0; /* a(n-2) */
    seq_t a2 = 1; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* c1 = 3*(4*i-5) */
        seq_t c1 = (seq_t)3 * ((seq_t)4 * i - 5);
        /* c2 = 48*i*i - 168*i + 151  (positive) */
        seq_t c2 = (seq_t)48 * (seq_t)i * (seq_t)i - (seq_t)168 * (seq_t)i + 151;
        /* c3 = (4*i-9)^3 */
        seq_t t  = (seq_t)4 * i - 9;
        seq_t c3 = t * t * t;

        /* a(i) = c1*a2 - c2*a1 + c3*a0 */
        seq_t val = c1 * a2;
        val = val - (c2 * a1);
        val = val + (c3 * a0);

        a0 = a1;
        a1 = a2;
        a2 = val;
    }

    return a2;
}
