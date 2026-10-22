/*
 * A381594  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of B(x)^k, where B(x)
 *          is the g.f. of A381601.
 * https://oeis.org/A381594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 63, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _antidiagonal_d, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A381594 –  Square array A(n,k) – read by antidiagonals
 *
 *  The sequence is obtained as follows:
 *      A(0,0)        = 1
 *      A(n,0)        = 0^n                               (so A(n,0)=0 for n>0)
 *      A(n,k) (k>0)  = k * Σ_{j=0..n} [ binomial(3n + j + k , j) /
 *                                       (3n + j + k) ] * A(n-j, 3j)
 *
 *  The sequence a(n) is obtained by traversing the array along antidiagonals
 *  downwards, i.e. for each d=0,1,2,… print A(0,d), A(1,d-1), … , A(d,0).
 *
 *  Initial values:
 *      a(0)=1 , a(1)=1 , a(2)=0 , a(3)=1 , a(4)=1 , a(5)=0 , a(6)=1 ,
 *      a(7)=2 , a(8)=7 , a(9)=0 , a(10)=1 , a(11)=3 , a(12)=15 , a(13)=79 ,
 *      a(14)=0 , a(15)=1 , a(16)=4 , a(17)=24 , a(18)=172 , a(19)=1134 ,
 *      a(20)=0 , a(21)=1 , a(22)=5 , a(23)=34
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/_antidiagonal_d.h"

/* Return the n-th term a(n). */
seq_t a(idx_t n)
{
    /* Determine which antidiagonal contains n. */
    idx_t d, count;
    _antidiagonal_d(n, &d, &count);

    /* The width of each row: we need columns up to 3*d. */
    size_t width = 3 * d + 1;

    /* Allocate the table A[0..d][0..3*d].  Zero-initialise. */
    seq_t **A = (seq_t**)malloc((d + 1) * sizeof(seq_t*));
    for (idx_t i = 0; i <= d; ++i) {
        A[i] = (seq_t*)calloc(width, sizeof(seq_t));
    }

    /* Compute values according to the recurrence. */
    for (idx_t i = 0; i <= d; ++i) {
        if (i == 0) {
            A[0][0] = (seq_t)1;          /* A(0,0)=1 */
        } else {
            A[i][0] = (seq_t)0;          /* A(n,0)=0 for n>0 */
        }

        for (idx_t k = 1; k < width; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j <= i; ++j) {
                seq_t bin = binomial(3 * i + j + k, j);      /* \binom{3i+j+k}{j} */
                seq_t prev = A[i - j][3 * j];                 /* A(n-j,3j) */
                seq_t term = bin * prev;                      /* bin * A */
                term = term * (seq_t)k;                      /* *k */
                seq_t denom = (seq_t)(3 * i + j + k);         /* division by 3i+j+k */
                term = term / denom;                          /* integer division */
                sum = sum + term;
            }
            A[i][k] = sum;
        }
    }

    /* Position within the diagonal: n = count + pos ,  pos = n - count */
    idx_t pos  = n - count;
    idx_t col  = d - pos;                 /* index (n,k) -> (pos, col) */
    seq_t result = A[pos][col];

    /* Free the table. */
    for (idx_t i = 0; i <= d; ++i) free(A[i]);
    free(A);

    return result;
}
