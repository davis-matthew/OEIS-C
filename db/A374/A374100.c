/*
 * A374100  Number of solutions to n^2 = x^2 + 3*x*y + y^2 with 0 < x < y.
 * https://oeis.org/A374100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x, y) with 0 < x < y to the
 *            Diophantine equation n² = x² + 3xy + y². Equivalent to counting
 *            pairs (a, b) with a² – 5b² = 4n², 3b < a < 5b and a ≡ b (mod 2),
 *            where a = 2x + 3y , b = y
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_sq = (seq_t)n * n;          /* n² */

    /* loop over all possible y = b (y > 0 and y < n).
       For y = n, a would equal 3n, violating the strict inequality a > 3b. */
    for (idx_t b = 1; b < n; ++b)
    {
        /* a² = 4n² + 5b² is the squared value we need to test. */
        seq_t rhs = 4 * n_sq + 5 * (seq_t)b * b;
        seq_t a_val = floor_sqrt(rhs);

        /* Check that rhs is a perfect square. */
        if (a_val * a_val != rhs)
            continue;

        /* Parity condition: a ≡ b (mod 2). */
        if ((a_val & 1) != ((seq_t)b & 1))
            continue;

        /* Verify the bounds 3b < a < 5b, guaranteeing
           0 < x < y with x = (a – 3b)/2. */
        if (3 * (seq_t)b < a_val && a_val < 5 * (seq_t)b)
            ++count;
    }

    return count;
}
