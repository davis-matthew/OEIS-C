/*
 * A285072  Triangle read by rows: coefficients of the Laplacian polynomial of
 *          the n-path graph P_n.
 * https://oeis.org/A285072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   T(n,k) = (-1)^(k+1)*binomial[n+k,2*k+1]; 0 <= n <= k
 * Range:     i8 n <= 23, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A285072 (triangle read by rows). */

#include <stdint.h>

/* Compute C(N,K) for idx_t arguments.  The algorithm uses the
   multiplicative formula C(N,K) = product_{i=1}^K (N-K+i)/i
   which keeps intermediate results integers. */
static inline seq_t binom_idx(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num   = (seq_t)(N - K + i);
        seq_t denom = (seq_t)i;
        res = res * num;
        res = res / denom;             /* division is exact by construction */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for offset 0 but safe */

    /* Find the row r such that r*(r+1)/2 >= n.  Row r has r terms. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Number of terms before row r: (r-1)*r/2  */
    idx_t prev = (r - 1) * r / 2;

    /* 0‑based column index within the row */
    idx_t k = n - prev - 1;

    /* 0‑based row index for the binomial expression */
    idx_t row = r - 1;

    /* Sign: (-1)^(k+1)  */
    seq_t sign = ((k + 1) & 1) ? (seq_t)-1 : (seq_t)1;

    /* Compute binomial coefficient C(row + k, 2*k + 1) */
    seq_t bin = binom_idx(row + k, 2 * k + 1);

    return sign * bin;
}
