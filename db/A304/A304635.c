/*
 * A304635  Triangle T(n,j) read by rows: the number of j-faces in the
 *          hypersimplicial decomposition of the unit cube of n dimensions.
 * https://oeis.org/A304635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = j*2^(r-j-1)*(r+j+2)*binomial(r+1,j+1)/(r+1) where r =
 *            smallest integer with r*(r+1)/2 >= n j = n - r*(r-1)/2. For j ==
 *            r the term equals r (avoids 2^(-1))
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

static idx_t _row_of(idx_t n)
{
    idx_t lo = 1, hi = n;               /* r ≤ n for any n ≥ 1       */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t N)
{
    if (N == 0) return (seq_t)0;        /* undefined for offset 1      */

    idx_t r = _row_of(N);
    idx_t j = N - (r - 1) * r / 2;      /* column index in the triangle */

    /* Special case j == r: T(r,r) = r */
    if (j == r) return (seq_t)r;

    seq_t jj  = (seq_t)j;
    seq_t nn  = (seq_t)r;
    seq_t bin = binomial(r + 1, j + 1);
    seq_t pw  = pow2(r - j - 1);        /* r - j - 1 ≥ 0 because j < r */

    seq_t num = jj * (nn + jj + 2);
    num = num * bin;
    num = num * pw;

    seq_t denom = (seq_t)(r + 1);
    return num / denom;
}
