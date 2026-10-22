/*
 * A301942  Expansion of e.g.f. arcsin(x)/cos(x) (odd powers only).
 * https://oeis.org/A301942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)! * [x^(2*n+1)] arcsin(x)/cos(x)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* compute up to coefficient of x^(2n+1) in arcsin(x)/cos(x) */
    idx_t M = 2 * n + 1;                /* highest exponent needed   */
    size_t len = (size_t)M + 1;         /* array length in elements   */

    /* allocate an array of seq_t coefficients S[0..M]   */
    seq_t *S;
    #ifdef __cplusplus
    S = new seq_t[len];
    #else
    S = (seq_t *)calloc(len, sizeof(seq_t));
    #endif
    S[0] = 0;                            /* coefficient of x^0 is zero */

    seq_t U = 1;                          /* value of (2k)!^2/(k!^2*4^k) for odd m = 2k+1   */

    for (size_t m = 1; m <= M; ++m)
    {
        seq_t T = 0;
        if (m & 1U)         /* odd m : use U = (2k)!^2/(k!^2*4^k)          */
        {
            T = U;
            U = U * ((seq_t)m * (seq_t)m);   /* next odd step: multiply by m^2   */
        }

        /* Sum over j = 1 .. floor(m/2) of (-1)^{j+1} * binom(m,2j) * S[m-2j] */
        seq_t pos = T;          /* positive contributions                      */
        seq_t neg = 0;          /* negative contributions                      */
        seq_t binom = 1;        /* binom(m,0)                                 */

        size_t maxj = m / 2;
        for (size_t r = 1; r <= maxj; ++r)
        {
            /* update binom to binom(m,2r) from binom(m,2r-2) */
            seq_t num1 = m - 2 * r + 2;
            seq_t num2 = m - 2 * r + 1;
            seq_t den1 = 2 * r - 1;
            seq_t den2 = 2 * r;

            binom = binom * num1;
            binom = binom * num2;
            binom = binom / den1;
            binom = binom / den2;

            seq_t term = binom * S[(size_t)m - 2 * r];
            if (r & 1U)      /* r odd -> positive sign                       */
                pos += term;
            else             /* r even -> negative sign                       */
                neg += term;
        }

        /* S[m] = pos - neg   (pos >= neg by construction) */
        if (pos < neg)
            S[m] = 0;
        else
            S[m] = pos - neg;
    }

    seq_t result = S[M];

    #ifdef __cplusplus
    delete[] S;
    #else
    free(S);
    #endif

    return result;          /* this equals a(n)                         */
}
