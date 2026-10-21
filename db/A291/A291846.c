/*
 * A291846  Central terms in irregular triangle A291845.
 * https://oeis.org/A291846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=0}^{n-1} (1 + (2k+1)x + x^2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* maximum n expected in tests; array size 20001 is enough for n ≤ 10000 */
#define MAX_N 20000
static seq_t coeff[MAX_N + 1];

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n > MAX_N)
        /* Fallback: compute with a temporary array if n exceeds MAX_N.
           In practice n will not reach this branch for the tested ranges. */
    {
        seq_t *tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = (seq_t)0;
        tmp[0] = (seq_t)1;

        for (idx_t k = 0; k < n; ++k)
        {
            seq_t c = (seq_t)(2 * k + 1);
            for (idx_t d = n; d >= 1; --d)
            {
                if (d >= 2)
                    tmp[d] = tmp[d] + tmp[d - 1] * c + tmp[d - 2];
                else
                    tmp[d] = tmp[d] + tmp[d - 1] * c;
            }
        }
        seq_t result = tmp[n];
        free(tmp);
        return result;
    }

    /* initialise up to index n */
    for (idx_t i = 0; i <= n; ++i)
        coeff[i] = (seq_t)0;
    coeff[0] = (seq_t)1;

    for (idx_t k = 0; k < n; ++k)
    {
        seq_t c = (seq_t)(2 * k + 1);
        for (idx_t d = n; d >= 1; --d)
        {
            if (d >= 2)
                coeff[d] = coeff[d] + coeff[d - 1] * c + coeff[d - 2];
            else
                coeff[d] = coeff[d] + coeff[d - 1] * c;
        }
    }
    return coeff[n];
}
