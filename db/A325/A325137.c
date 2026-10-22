/*
 * A325137  Triangle T(n, k) = [x^n] (n + k + x)!/(k + x)! for 0 <= k <= n,
 *          read by rows.
 * https://oeis.org/A325137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): read by rows the triangle T(n,k) =
   Sum_{j=0..n-k} binomial(j+k,k)*|Stirling1(n,j+k)|*(k+1)^j   */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"      /* binomial */
#include "../utils/big_pow.h"      /* big_pow */

/* Maximum n for which we need Stirling numbers.  For n <= 10000
   the corresponding row index never exceeds 140, so 200 is safe. */
#define MAX_M 200

static seq_t stirling1[MAX_M][MAX_M];
static bool  stirling_init = false;
static idx_t stirling_max  = 0;

/* Recursively fill Stirling numbers of the first kind up to given max n */
static void compute_stirling_up_to(idx_t n)
{
    if (!stirling_init) {
        stirling1[0][0] = 1;
        stirling_init = true;
        stirling_max  = 0;
    }
    if (n <= stirling_max) return;

    for (idx_t i = stirling_max + 1; i <= n; ++i) {
        stirling1[i][0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t a = stirling1[i-1][k-1];
            seq_t b = stirling1[i-1][k] * (seq_t)(i-1);
            stirling1[i][k] = a + b;
        }
    }
    stirling_max = n;
}

/* Compute T(m,r) where 0 <= r <= m  */
static seq_t T_value(idx_t m, idx_t r)
{
    compute_stirling_up_to(m);
    seq_t sum = 0;
    idx_t maxj = m - r;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t binom = binomial(j + r, r);
        seq_t stir  = stirling1[m][j + r];
        seq_t powv  = big_pow((seq_t)(r + 1), j);

        /* Use 128–bit intermediates to avoid overflow when seq_t is small. */
        unsigned __int128 prod = (unsigned __int128)binom * (unsigned __int128)stir;
        prod = prod * (unsigned __int128)powv;
        seq_t term = (seq_t)prod;
        sum += term;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while ((m * (m + 1) / 2) <= n) ++m;
    --m;                         /* now m satisfies the condition */

    idx_t tri = m * (m + 1) / 2;  /* triangular number for row m */
    idx_t r   = n - tri;          /* column within the row */

    return T_value(m, r);
}
