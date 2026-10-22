/*
 * A362924  Triangle read by rows: T(n,m), n >= 1, 1 <= m <= n, is number of
 *          partitions of the set {1,2,...,n} that have at most one block
 *          contained in {1,...,m}.
 * https://oeis.org/A362924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,m) where r = smallest integer with r(r+1)/2 ≥ n * and
 *            m = n − r(r−1)/2 (1 ≤ m ≤ r). * T(r,m) = Σ_{k=0}^{r−m}
 *            Stirling_2(r−m,k)·(k+1)^m * (Stirling_2 are Stirling numbers of
 *            the second kind)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 120, u64 n <= 325,
 *            u128 n <= 903, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t S[150][150];          /* Stirling numbers S[n][k]  */
static idx_t Smax = 0;             /* Largest n whose row is computed */

/* Build Stirling numbers of the second kind up to row 'up_to'. */
static void build_stirling(idx_t up_to)
{
    if (up_to <= Smax) return;
    if (Smax == 0) S[0][0] = (seq_t)1;          /* Base case */

    for (idx_t n = Smax + 1; n <= up_to; ++n) {
        S[n][0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = S[n-1][k] * (seq_t)k;
            seq_t term2 = S[n-1][k-1];
            S[n][k] = term1 + term2;
        }
    }
    Smax = up_to;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* Find minimal r such that r(r+1)/2 ≥ n.  r is at most about 141
       for n ≤ 10000, so a simple loop is fine. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column index m. */
    idx_t m = n - r * (r - 1) / 2;

    /* Ensure Stirling numbers computed up to row r. */
    build_stirling(r);

    /* Compute T(r,m). */
    seq_t sum = (seq_t)0;
    idx_t n_m = r - m;                     /* n − m */
    for (idx_t k = 0; k <= n_m; ++k) {
        seq_t stir = S[n_m][k];
        if (stir == (seq_t)0) continue;     /* skip zero terms if desired */
        seq_t pow_val = big_pow((seq_t)(k + 1), (seq_t)m);
        sum += stir * pow_val;
    }
    return sum;
}
