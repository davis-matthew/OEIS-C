/*
 * A281183  E.g.f. C(x)^2 = cosh( Integral C(x)^3 dx )^2 where C(x) is
 *          described by A281181.
 * https://oeis.org/A281183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281183
   a(0) = 1
   a(n) = Σ_{k=0}^{n-1} binom(2n,2k+1) * s(k) * s(n-1-k)
   s(n) = Σ_{k=0}^{n}   binom(2n,2k)   * a(k) * a(n-k)
   (s(0)=1).  Coefficients are computed in O(n²) time and O(n) memory.
*/

#include <stdlib.h>
#include <stdint.h>

/* external integer types supplied by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;

    /* allocate arrays of length n+1 */
#ifdef __cplusplus
    seq_t *a_arr = new seq_t[n + 1]();
    seq_t *s_arr = new seq_t[n + 1]();
#else
    seq_t *a_arr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *s_arr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (!a_arr || !s_arr) return (seq_t)0;   /* allocation failure */

    a_arr[0] = (seq_t)1;
    s_arr[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m)
    {
        /* compute a[m] */
        seq_t sumA = 0;
        seq_t bin = 2 * m;                       /* binom(2m,1) */
        sumA += s_arr[0] * s_arr[m - 1] * bin;

        for (idx_t k = 1; k < m; ++k)
        {
            idx_t r = 2 * k - 1;                 /* current odd index */
            seq_t tmp = bin;
            tmp = tmp * (seq_t)(2 * m - r) / (seq_t)(r + 1);
            tmp = tmp * (seq_t)(2 * m - r - 1) / (seq_t)(r + 2);
            bin = tmp;

            sumA += s_arr[k] * s_arr[m - 1 - k] * bin;
        }
        a_arr[m] = sumA;

        /* compute s[m] */
        seq_t sumS = 0;
        bin = 1;                                 /* binom(2m,0) */
        for (idx_t i = 0; i <= m; ++i)
        {
            sumS += a_arr[i] * a_arr[m - i] * bin;
            if (i < m)
            {
                idx_t r = 2 * i;                 /* current even index */
                seq_t tmpS = bin;
                tmpS = tmpS * (seq_t)(2 * m - r) / (seq_t)(r + 1);
                tmpS = tmpS * (seq_t)(2 * m - r - 1) / (seq_t)(r + 2);
                bin = tmpS;
            }
        }
        s_arr[m] = sumS;
    }

    seq_t result = a_arr[n];

#ifdef __cplusplus
    delete[] a_arr;
    delete[] s_arr;
#else
    free(a_arr);
    free(s_arr);
#endif
    return result;
}
