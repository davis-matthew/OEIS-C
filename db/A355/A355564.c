/*
 * A355564  Triangle read by rows: T(n,k) = n*(1+2*k) - k*(1+k), n >= 1, 0 <=
 *          k <= n-1.
 * https://oeis.org/A355564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) with * T(r,k) = r*(1+2*k) - k*(1+k) * r =
 *            ceil((sqrt(8*n+1)-1)/2) * k = n - r*(r-1)/2 - 1 * Both r and k
 *            are 0‑based indices of the triangle rows and columns
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x <= 2^64-1. */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* largest power of 4 <= 2^64 */
    idx_t bit = 1ULL << 62;
    while (bit > x)
        bit >>= 2;
    for (; bit != 0; bit >>= 2) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* n is 1‑based. */
    if (n == 0)          /* offset 1: undefined behaviour below 1, but return 0 */
        return (seq_t)0;

    /* compute r = ceil((sqrt(8*n+1)-1)/2) without floating point */
    idx_t u = 8 * n + 1;
    idx_t s = floor_sqrt_u64(u);

    idx_t r = (s - 1) / 2;          /* floor(..) */
    if (r * (r + 1) / 2 < n)        /* adjust if needed */
        r++;

    /* compute k = n - triangular(r-1) - 1 */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri - 1;

    /* compute T(r,k) = r*(1 + 2*k) - k*(1 + k) */
    seq_t rseq = (seq_t)r;
    seq_t kseq = (seq_t)k;
    seq_t res = rseq * (1 + 2 * kseq) - kseq * (1 + kseq);

    return res;
}
