/*
 * A249267  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation Sum_{k=0..n} x^k = Sum_{k=0..n} A_k*(x-2*(-1)^k)^k.
 * https://oeis.org/A249267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A249267 : coefficients of the transform  
 *  1 + x + … + x^n = Σk A_k (x - 2*(-1)^k)^k
 *  The sequence a(n) is read row‑wise from the triangle
 *  T(n,k)=A_k of coefficients.
 *
 *  Recurrence for a row of length r (index 0..r):
 *      A_r = 1
 *      for j = r-1 … 0:
 *          A_j = 1 - Σ_{k=j+1..r} A_k * C(k,j) * 2^{k-j} * (-1)^{(k+1)(k-j)}
 *
 *  The function computes the nth element of the flattened row‑wise
 *  sequence.  It works for any seq_t/idx_t supplied by the build,
 *  using only integer arithmetic.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine the row r for index n (0‑based) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* index within the row */
    idx_t offset = n - r * (r + 1) / 2;

    /* allocate arrays for coefficients and powers of two */
    seq_t *A = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *pow2 = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!A || !pow2) {
        free(A);
        free(pow2);
        return 0; /* allocation failed */
    }

    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* base case */
    if (r == 0) {
        *A = (seq_t)1;
    } else {
        A[r] = (seq_t)1;
        /* compute backwards from j = r-1 down to 0 */
        for (idx_t j = r; j-- > 0; ) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1; /* C(j,j) = 1 */
            for (idx_t k = j + 1; k <= r; ++k) {
                /* update binom to C(k,j) */
                binom = binom * (seq_t)k;
                binom = binom / (seq_t)(k - j);

                seq_t term = A[k] * binom;
                term = term * pow2[k - j];

                /* sign = (-1)^{(k+1)(k-j)} , negative when both (k+1) and (k-j) odd */
                if (((k + 1) & 1UL) && ((k - j) & 1UL)) {
                    term = -term;
                }
                sum += term;
            }
            A[j] = (seq_t)1 - sum;
        }
    }

    seq_t result = A[offset];
    free(A);
    free(pow2);
    return result;
}
