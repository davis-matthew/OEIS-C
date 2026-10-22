/*
 * A342967  a(n) = 1 + Sum_{j=1..n} Product_{k=0..j-1} binomial(2*n-1,n+k) /
 *          binomial(2*n-1,k).
 * https://oeis.org/A342967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{j=1..n} Π_{k=0..j-1} binomial(2*n-1,
 *            n+k)/binomial(2*n-1, k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  (a(0)=1, a(1)=2, a(2)=5, ...) */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* N = 2*n - 1 */
    idx_t N = 2 * n - 1;

    /* Compute C(N, n) iteratively */
    seq_t C_n = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        C_n = C_n * (seq_t)(N - n + k) / (seq_t)k;
    }

    /* C_left = C(N, j), starting with j = 0 */
    seq_t C_left = (seq_t)1;

    /* C_right = C(N, n + j), starting with j = 0 */
    seq_t C_right = C_n;

    seq_t sum   = (seq_t)1;   /* contribution of j = 0 (empty product) */
    seq_t P_j   = (seq_t)1;   /* P_0 = 1 */

    for (idx_t j = 0; j < n; ++j) {
        /* Next term: P_{j+1} = P_j * C_right / C_left (exact integer) */
        seq_t num = C_right;
        seq_t den = C_left;

        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd(P_j, den);
        P_j /= g2;
        den /= g2;

        /* Now den divides P_j * num exactly */
        seq_t nextP = P_j * num / den;
        sum += nextP;
        P_j = nextP;

        /* Update binomials for next j */
        /* C_left for j+1: C(N, j+1) = C_left * (N - j) / (j + 1) */
        C_left = C_left * (seq_t)(N - j) / (seq_t)(j + 1);

        /* C_right for j+1: C(N, n + j + 1) = C_right * (N - n - j) / (n + j + 1) */
        C_right = C_right * (seq_t)(N - n - j) / (seq_t)(n + j + 1);
    }

    return sum;
}
