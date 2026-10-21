/*
 * A261615  Expansion of Product_{k>=0} (1 + x^(3*k+1))^2.
 * https://oeis.org/A261615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in * ∏_{k≥0} (1 + x^(3k+1))^2 * =
 *            ∏_{k≥0} (1 + 2x^(3k+1) + x^(2·(3k+1)))
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* temporary coefficient array */
    seq_t *coeff = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!coeff)
        return (seq_t)0;   /* allocation failure – unlikely for n ≤ 10000 */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i)
        coeff[i] = (seq_t)0;
    coeff[0] = (seq_t)1;

    /* multiply by each factor (1 + 2x^a + x^{2a}) with a = 3k+1 */
    for (idx_t a = 1; a <= n; a += 3)
    {
        /* descending loop guarantees original coeff[i] values are used */
        for (idx_t i = n; ; --i)
        {
            if (i + a <= n)
                coeff[i + a] += (seq_t)2 * coeff[i];
            if (i + 2 * a <= n)
                coeff[i + 2 * a] += coeff[i];
            if (i == 0) break;
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
