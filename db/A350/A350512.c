/*
 * A350512  Triangle read by rows with T(n,0) = 1 for n >= 0 and T(n,k) =
 *          binomial(n-1,k-1)*(2*k*(n-k) + n)/k for 0 < k <= n.
 * https://oeis.org/A350512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k), where r is the largest integer with r(r+1)/2 <= n
 *            * and k = n - r(r+1)/2. T(r,0)=1 and for 0<k<=r, *
 *            T(r,k)=C(r-1,k-1) * (2*k*(r-k)+r) / k. * This implements the
 *            definition from OEIS A350512
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t, idx_t); */

static idx_t triangular_root(idx_t n)
{
    /* Find largest r with r*(r+1)/2 <= n.  Complexity O(sqrt(n)). */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

/* Compute T(r,k) for 0 <= k <= r. */
static seq_t T_row_k(idx_t r, idx_t k)
{
    if (k == 0)
        return (seq_t)1;

    /* binomial(r-1, k-1) */
    seq_t bin = binomial(r - 1, k - 1);

    /* numerator: 2*k*(r-k)+r */
    seq_t numer = (seq_t)((seq_t)2 * k * (r - k) + r);

    /* product and division (exact). */
    seq_t prod = bin * numer;
    seq_t result = prod / k;

    return result;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_root(n);
    idx_t k = n - r * (r + 1) / 2;

    return T_row_k(r, k);
}
