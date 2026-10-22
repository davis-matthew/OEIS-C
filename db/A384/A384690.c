/*
 * A384690  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B(x)^k, where
 *          B(x) is the e.g.f. of A384689.
 * https://oeis.org/A384690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = k * Sum_{j=1..n} (n+j+k)^(j-1) * binomial(n,j) *
 *            A(n-j,j) with A(0,k)=1 and A(n,0)=0 for n>0. The sequence a(n)
 *            is obtained by reading the square array A(n,k) by antidiagonals
 *            downwards
 * Range:     u8 n <= 18, u16 n <= 25, u32 n <= 52, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_power.h"

 /*------------------ auxiliary arithmetic ----------------------------*/

static seq_t _binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

/*------------------ dynamic triangle storage -----------------------*/

static seq_t **A = NULL;      /* A[n][k] for all 0 ≤ n+k ≤ dmax   */
static idx_t dmax = 0;        /* largest diagonal computed so far */

/* ensure that all values up to diagonal d exist */
static void _ensure_d(idx_t d)
{
    /* nothing to do if already available */
    if (A != NULL && d <= dmax) return;

    /* allocate new matrix of size (d+1)×(d+1) */
    seq_t **newA;
#if defined(__cplusplus)
    newA = new seq_t*[d + 1];
    for (idx_t i = 0; i <= d; ++i)
        newA[i] = new seq_t[d + 1];
#else
    newA = (seq_t**)malloc((d + 1) * sizeof(seq_t*));
    for (idx_t i = 0; i <= d; ++i)
        newA[i] = (seq_t*)calloc(d + 1, sizeof(seq_t));
#endif

    /* copy old rows (if any) */
    if (A != NULL) {
        for (idx_t i = 0; i <= dmax; ++i)
            for (idx_t j = 0; j <= dmax; ++j)
                newA[i][j] = A[i][j];

        /* free old storage */
        for (idx_t i = 0; i <= dmax; ++i) {
#if defined(__cplusplus)
            delete[] A[i];
#else
            free(A[i]);
#endif
        }
#if defined(__cplusplus)
        delete[] A;
#else
        free(A);
#endif
    }

    /* replace old matrix with the new one */
    A = newA;

    /* compute values for all pairs with n + k ≤ d */
    for (idx_t n = 0; n <= d; ++n) {
        for (idx_t k = 0; k + n <= d; ++k) {
            if (n == 0) {
                A[0][k] = (seq_t)1;            /* A(0,k)=1  */
            } else if (k == 0) {
                A[n][0] = (seq_t)0;            /* A(n,0)=0  */
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= n; ++j) {
                    seq_t term = _power((seq_t)(n + j + k), j - 1);
                    term = term * _binomial(n, j);
                    term = term * A[n - j][j];
                    sum += term;
                }
                A[n][k] = sum * (seq_t)k;
            }
        }
    }
    dmax = d;
}

/*------------------ public interface --------------------------------*/

seq_t a(idx_t n)
{
    /* determine diagonal d such that T(d) ≤ n < T(d+1) */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n) ++d;

    /* ensure triangle up to diagonal d is ready */
    _ensure_d(d);

    /* offset along the diagonal: row index starts at 0 */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t row = offset;
    idx_t col = d - offset;

    return A[row][col];
}
