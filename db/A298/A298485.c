/*
 * A298485  Triangle read by rows; row 0 is 1; the n-th row for n>0 contains
 *          the coefficients in the expansion of (2-x)*(1+x)^(n-1).
 * https://oeis.org/A298485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient of x^k in the expansion of
 *            (2-x)*(1+x)^(r-1), where r is the unique row with r*(r+1)/2 ≤ n
 *            < (r+1)*(r+2)/2 (i.e. the row that contains the n‑th element
 *            when the triangle is read by rows). Equivalently, T(0,0)=1,
 *            T(1,0)=2, T(1,1)=-1, T(n,k)=T(n-1,k-1)+T(n-1,k)
 * Range:     i8 n <= 57, i16 n <= 196, i32 n <= 644, i64 n <= 2374,
 *            i128 n <= 8838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row containing index n: largest r such that r*(r+1)/2 <= n. */
static idx_t
find_row(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 0, hi = 1;
    /* Double hi until its triangular number exceeds n. */
    while (((__int128)hi * (hi + 1)) / 2 <= n)
        hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Compute binomial coefficient C(N,K) for unsigned N,K
   using multiplicative formula with 128‑bit intermediate to avoid overflow. */
static seq_t
binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry */
    __int128 res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (N - i + 1);
        res = res / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* row 0, the single element 1 */

    idx_t r = find_row(n);                              /* row index (>=1) */
    idx_t k = n - (idx_t)(((__int128)r * (r + 1)) / 2); /* column index */

    seq_t bin1 = binom(r - 1, k);                        /* C(r-1,k)   */
    seq_t bin2 = (k == 0) ? 0 : binom(r - 1, k - 1);    /* C(r-1,k-1) */

    __int128 tmp = (__int128)bin1 * 2 - (__int128)bin2;
    return (seq_t)tmp;
}
