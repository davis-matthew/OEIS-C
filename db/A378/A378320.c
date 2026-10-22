/*
 * A378320  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,0) = 0^n and T(n,k) = k * Sum_{r=0..n}
 *          binomial(2*n-2*r+k,r) * binomial(r,n-r)/(2*n-2*r+k) for k > 0.
 * https://oeis.org/A378320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(n,k) * defined by T(n,0) =
 *            0^n, * T(0,k) = 1, * T(1,k) = k (k≥1), * T(n,k) = T(n,k-1) +
 *            T(n-1,k-1) + T(n-2,k+1) for n≥2, k≥1
 * Range:     u8 n <= 60, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any unsigned 64‑bit index n. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- find the diagonal that contains index n ---------- */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;          /* largest s with s*(s+1)/2 ≤ n */

    idx_t start  = s * (s + 1) / 2;                   /* first index of this diagonal */
    idx_t offset = n - start;                         /* position inside the diagonal    */
    idx_t row    = offset;                            /* n component on the diagonal     */
    idx_t col    = s - offset;                        /* k component on the diagonal     */

    /* ---------- build the triangular prefix of the array ---------- */
    idx_t stride = s + 1;                              /* table width/height */
    size_t tblsize = (size_t)stride * (size_t)stride;  /* total cells needed   */
    seq_t *data = (seq_t *)calloc(tblsize, sizeof(seq_t));
    if (!data) return 0;                              /* allocation failure    */

    for (idx_t i = 0; i <= s; ++i)
    {
        idx_t maxk = s - i;                           /* largest k for this row     */
        for (idx_t j = 0; j <= maxk; ++j)
        {
            if (i == 0)                                 /* first row: all 1 */
            {
                data[i * stride + j] = (seq_t)1;
            }
            else if (j == 0)                           /* first column: 0 (except (0,0) already set) */
            {
                data[i * stride + j] = (seq_t)0;
            }
            else if (i == 1)                           /* row 1: k (k≥1) */
            {
                data[i * stride + j] = (seq_t)j;
            }
            else                                        /* recurrence for n≥2, k≥1 */
            {
                seq_t t1 = data[i * stride + (j - 1)];        /* T(n,k-1)      */
                seq_t t2 = data[(i - 1) * stride + (j - 1)];  /* T(n-1,k-1)    */
                seq_t t3 = data[(i - 2) * stride + (j + 1)];  /* T(n-2,k+1)    */
                data[i * stride + j] = t1 + t2 + t3;
            }
        }
    }

    seq_t result = data[row * stride + col];
    free(data);
    return result;
}
