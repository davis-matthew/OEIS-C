/*
 * A369324  Array read by ascending antidiagonals: A(n,k) is the number of
 *          words of length n on an alphabet [k], avoiding 120 and 210, and
 *          sortable by a stack of depth 2, where k >= 0.
 * https://oeis.org/A369324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array A(n,k) by ascending
 *            antidiagonals. A(n,k) = (k&1) + 2^n * Σ_{m=0..⌊(k-2)/2⌋}
 *            binomial(n + k - 3 - 2m , n-1), with the convention that
 *            binomial(x,y)=0 unless 0≤y≤x and that for n=0 the sum
 *            contributes 1 iff k is even and ≥2
 * Range:     u8 n <= 47, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* locate (n_,k_) on the antidiagonal of index n */
    idx_t t = n;
    /* solve s*(s+1)/2 <= t < (s+1)*(s+2)/2 */
    double d = sqrt((double)1 + 8.0 * (double)t);
    idx_t s = (idx_t)((d - 1.0) / 2.0);
    /* adjust for possible off‑by‑one due to floating errors */
    while (s * (s + 1) / 2 > t) --s;
    while ((s + 1) * (s + 2) / 2 <= t) ++s;

    idx_t pos = t - s * (s + 1) / 2;
    idx_t n_ = s - pos;   /* first coordinate (row) */
    idx_t k_ = pos;       /* second coordinate (column) */

    /* compute A(n_,k_) */
    seq_t parity = ((k_ & 1ULL) != 0) ? 1 : 0;
    seq_t sum = 0;

    if (k_ >= 2) {
        if (n_ == 0) {
            /* sum is 1 iff k_ is even and ≥2 */
            if ((k_ & 1ULL) == 0) sum = 1;
        } else {          /* n_ >= 1 */
            idx_t bottom = n_ - 1;
            idx_t max_i = (k_ - 2) / 2;
            for (idx_t m = 0; m <= max_i; ++m) {
                idx_t top = n_ + k_ - 3 - 2 * m;
                if (top < bottom) continue;   /* binomial zero */
                sum += binomial(top, bottom);
            }
        }
    }

    seq_t pow2_n = pow2(n_);
    seq_t result = parity + pow2_n * sum;
    return result;
}
