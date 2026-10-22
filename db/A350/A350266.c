/*
 * A350266  Triangle read by rows. T(n, k) = binomial(n, k) * n! / (n - k +
 *          1)! if k >= 1, if k = 0 then T(n, k) = k^n. T(n, k) for 0 <= k <=
 *          n.
 * https://oeis.org/A350266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,c) read triangle where * T(r,0) = 0^r (with 0^0 =
 *            1) * T(r,c) = binomial(r,c) * r! / (r-c+1)! for c >= 1 * Which
 *            simplifies to * T(r,c) = binomial(r,c) * Π_{t=r-c+2}^{r} t. *
 *            Offset 0 (a(0) = 1)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, index_in_triangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/index_in_triangle.h"

static inline seq_t triangle_value(idx_t r, idx_t k)
{
    if (k == 0)
        return (r == 0) ? 1 : 0;

    seq_t bin = binomial(r, k);
    seq_t prod = 1;
    for (idx_t t = r - k + 2; t <= r; ++t)
        prod *= (seq_t)t;
    return bin * prod;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    index_in_triangle(n, &r, &k);
    return triangle_value(r, k);
}
