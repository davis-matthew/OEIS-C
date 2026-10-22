/*
 * A327916  Triangle T(k, n) read by rows: Array A(k, n) = 2^k*(k + 1 + 2*n),
 *          k >= 0, n >= 0, read by antidiagonals upwards.
 * https://oeis.org/A327916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327916:  a(n) is obtained by reading the array
   A(k,n) = 2^k * (k + 1 + 2*n) in antidiagonal order,
   i.e. for each d = 0,1,2,... list the pairs (k = d .. 0),
   n' = d - k.  For a given index n:
      d  = floor((sqrt(8*n+1) - 1)/2)
      r  = n - d*(d+1)/2          // position within antidiagonal d
      k  = d - r
      m  = r
      a(n) = 2^k * (k + 1 + 2*m)  */

#include <stdint.h>

static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    if (hi == 0) return 0;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* compute d = floor((sqrt(8*n+1)-1)/2) */
    idx_t t = (idx_t)(((unsigned __int128)8 * n) + 1);
    idx_t sqrt_t = isqrt_idx(t);
    idx_t d = (sqrt_t - 1) / 2;

    /* offset inside the antidiagonal */
    idx_t before = d * (d + 1) / 2;
    idx_t r = n - before;

    /* indices in the array A(k,n) */
    idx_t k = d - r;
    idx_t m = r;

    /* compute 2^k */
    seq_t pow2_val = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2_val *= 2;

    /* compute the final value */
    seq_t sum = (seq_t)(k + 1 + 2 * m);
    return pow2_val * sum;
}
