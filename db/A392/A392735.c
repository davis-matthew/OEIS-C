/*
 * A392735  Triangle read by rows: T(n,k) = f(n,k,2) where f(n,k,m) =
 *          Sum_{j=k-1..n-1} binomial(n,j) * f(j+1,k,m-1) with f(n,k,0) = [n =
 *          k].
 * https://oeis.org/A392735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n – r(r-1)/2, and T(r,k) = Σ_{j=k-1}^{r-1} binomial(r,j) *
 *            binomial(j+1,k-1)
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t triangle_T(idx_t n, idx_t k)
{
    seq_t total = 0;
    for (idx_t j = k - 1; j <= n - 1; ++j) {
        seq_t t1 = binomial(n, j);
        seq_t t2 = binomial(j + 1, k - 1);
        total += t1 * t2;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset: sequence starts at n = 1 */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum = r * (r + 1) / 2;
    }
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;
    return triangle_T(r, k);
}
