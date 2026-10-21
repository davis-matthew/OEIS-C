/*
 * A240955  Triangular array read by rows. T(n,k) is the number of k-colored
 *          labeled digraphs with n vertices, n>=1, 1<=k<=n.
 * https://oeis.org/A240955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row number such that r*(r+1)/2 >=
 *            n, and k = n - r*(r-1)/2. For a digraph with r vertices and k
 *            colors: T(r,1) = 1 T(r,r) = r! * 2^(r^2 - r) For 1 < k < r:
 *            T(r,k) = sum_{s=1}^{r-k+1} C(r, s) * 2^(2*s*(r-s)) * T(r-s, k-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 32, u128 n <= 36,
 *            big any n
 * Helpers:   binom, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/factorial.h"
#include "../utils/pow2.h"

/* Maximum vertex count needed for indices up to 10000:
   200*201/2 = 20100 > 10000.  Store up to 200 rows. */
#define MAX_V 200
static seq_t cache[MAX_V + 1][MAX_V + 1];
static idx_t max_cached = 0;

seq_t a(idx_t n) {
    /* Determine the row r and column k corresponding to index n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;
    idx_t k = n - (r - 1) * r / 2;  /* offset within the row */

    /* Precompute values up to row r if necessary */
    if (r > max_cached) {
        for (idx_t i = max_cached + 1; i <= r; ++i) {
            cache[i][1] = 1;                     /* T(i,1) */
            for (idx_t kk = 2; kk < i; ++kk) {   /* 1 < kk < i */
                seq_t sum = 0;
                idx_t maxS = i - kk + 1;
                for (idx_t s = 1; s <= maxS; ++s) {
                    seq_t term = binom(i, s);
                    term *= pow2((idx_t)(2 * s * (i - s)));
                    term *= cache[i - s][kk - 1];
                    sum += term;
                }
                cache[i][kk] = sum;
            }
            cache[i][i] = factorial(i) * pow2(i * i - i);  /* T(i,i) */
        }
        max_cached = r;
    }

    return cache[r][k];
}
