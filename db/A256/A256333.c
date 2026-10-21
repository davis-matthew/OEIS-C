/*
 * A256333  Number of R&G Family matchings on n edges.
 * https://oeis.org/A256333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            the algebraic generating function f(x) defined by x^2 * f^4 + x
 *            * (1 - x)^2 * f^2 - (1 - x)^2 * f + (1 - x)^2 = 0 The sequence
 *            starts at n = 1 (a(1) = 1). The recurrence for the coefficients
 *            S_n (where S_n = a(n+1), S_0 = a(1) = 1) i...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n >= 1 */

    size_t N = (size_t)n;          /* number of terms to compute */
    seq_t *S   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *p2  = (seq_t *)calloc((N + 1), sizeof(seq_t));   /* convolution of f with itself */
    if (!S || !p2) return 0;      /* failure: do nothing special */

    /* initialise */
    S[0] = 1;                      /* a(1) */
    p2[0] = S[0] * S[0];           /* coefficient of x^0 in f^2 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute p4[i-2] = coefficient of x^{i-2} in f^4 = (f^2)^2 */
        seq_t p4 = 0;
        if (i >= 2)
        {
            for (idx_t k = 0; k <= i-2; ++k)
                p4 += p2[k] * p2[(i-2) - k];
        }

        seq_t p2_nm1 = (i >= 1) ? p2[i-1] : 0;
        seq_t p2_nm2 = (i >= 2) ? p2[i-2] : 0;
        seq_t p2_nm3 = (i >= 3) ? p2[i-3] : 0;

        seq_t val = p4;
        val += p2_nm1;
        val += p2_nm3;
        val += (seq_t)2 * S[(size_t)i-1];

        if (i >= 2) val -= S[(size_t)i-2];
        if (i >= 2) val -= (seq_t)2 * p2_nm2;

        if (i == 1) val -= (seq_t)2;       /* const term for n==1 */
        else if (i == 2) val += (seq_t)1;  /* const term for n==2 */

        S[(size_t)i] = val;

        /* update convolution array p2 with the new coefficient S[i] */
        for (idx_t j = 0; j < i; ++j)
        {
            size_t idx = (size_t)i + j;
            if (idx > N) break;
            p2[idx] += (seq_t)2 * S[(size_t)i] * S[(size_t)j];
        }
        size_t idx_ll = (size_t)i + i;     /* i + i part */
        if (idx_ll <= N)
            p2[idx_ll] += S[(size_t)i] * S[(size_t)i];
    }

    seq_t result = S[(size_t)n];
    free(S);
    free(p2);
    return result;
}
