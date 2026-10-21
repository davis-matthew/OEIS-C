/*
 * A264357  Array A(r, n) of number of independent components of a symmetric
 *          traceless tensor of rank r and dimension n, written as triangle
 *          T(n, r) = A(r, n-r+2), n >= 1, r = 2..n+1.
 * https://oeis.org/A264357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = A(r, n-r+2) where A(r,m) = binom(m+r-1, r) - binom(m+r-3,
 *            r-2). n is counted row‑major: row k contains k terms (for k≥1).
 *            r ranges from 2..k+1 in row k and m = k - r + 2
 * Range:     u8 n <= 47, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- standard includes ----------------------------------------------*/
#include <stdint.h>
/*--- utilities ------------------------------------------------------*/
#include "../utils/seq_gcd.h"

/*--- small integer sqrt (floor) -------------------------------------*/
static idx_t isqrt(idx_t v)
{
    idx_t low = 0, high = v;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/*--- find row number N such that N*(N+1)/2 ≥ n ---------------------*/
static idx_t find_row(idx_t n)
{
    /* approximate with integer sqrt of 8*n+1 */
    idx_t N = (isqrt(8 * n + 1) - 1) >> 1;
    if (N * (N + 1) / 2 < n)
        N += 1;
    return N;
}

/*--- binomial coefficient (N choose K) using seq_t -----------------*/
static seq_t binomial(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0 || K == N) return (seq_t)1;
    if (K > N - K) K = N - K;   /* symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i);
        seq_t den = (seq_t)i;
        seq_t g1 = seq_gcd(num, den);   /* reduce numerator/denominator */
        num /= g1;
        den /= g1;
        seq_t g2 = seq_gcd(result, den);/* reduce result/denominator */
        result /= g2;
        den /= g2;
        result *= num;                 /* den should now be 1 */
    }
    return result;
}

/*--- compute A(r,m) = binom(m+r-1,r) - binom(m+r-3,r-2) -----------*/
static seq_t A_r_m(idx_t r, idx_t m)
{
    seq_t term1 = binomial(m + r - 1, r);
    seq_t term2 = binomial(m + r - 3, r - 2);
    return term1 - term2;
}

/*--- sequence entry a(n) --------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* undefined, guard */
    idx_t row = find_row(n);                          /* row number k */
    idx_t start = (row - 1) * row / 2 + 1;            /* first index of row */
    idx_t offset = n - start;                         /* 0‑based offset in row */
    idx_t r = 2 + offset;                             /* column index r */
    idx_t m = row - r + 2;                            /* dimension index   */
    return A_r_m(r, m);
}
