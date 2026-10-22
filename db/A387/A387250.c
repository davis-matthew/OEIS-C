/*
 * A387250  a(n) = 105/(n + 1) * Catalan(4*n).
 * https://oeis.org/A387250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 105/(n + 1) * Catalan(4*n) Recurrence: a(n) = 2*(8*n -
 *            1)*(8*n - 3)*(8*n - 5)*(8*n - 7) / ((n + 1)*(2*n - 1)*(4*n +
 *            1)*(4*n - 1)) * a(n-1), a(0) = 105
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)105;

    seq_t result = (seq_t)105;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* numerator = 2*(8*i-1)*(8*i-3)*(8*i-5)*(8*i-7) */
        seq_t num = (seq_t)2 * ((seq_t)8 * i - 1);
        num *= ((seq_t)8 * i - 3);
        num *= ((seq_t)8 * i - 5);
        num *= ((seq_t)8 * i - 7);

        /* denominator = (i+1)*(2*i-1)*(4*i+1)*(4*i-1) */
        seq_t den = (seq_t)i + 1;
        den *= ((seq_t)2 * i - 1);
        den *= ((seq_t)4 * i + 1);
        den *= ((seq_t)4 * i - 1);

        /* Simplify the fraction before multiplying to avoid overflow
           (only necessary for 64‑bit builds). */
        seq_t g1 = gcd(result, den);
        result /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;   /* after cancellation den should be 1 */

        result *= num;
    }

    return result;
}
