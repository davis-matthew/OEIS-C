/*
 * A247588  Number of integer-sided acute triangles with largest side n.
 * https://oeis.org/A247588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor(n*(1 - sqrt(2)/2))} (n - j -
 *            floor(sqrt(2*j*n - j^2)))
 * Range:     u8 n <= 45, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for v up to 2^53.
   Uses simple binary search – safe for the ranges used here. */
static idx_t isqrt_idx(idx_t v)
{
    if (v < 2) return v;
    idx_t low = 1, high = 500000;          /* 500000^2 > 2^53, plenty for n≤10000 */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            ans = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* The upper limit j = floor(n*(1 - sqrt(2)/2)) can be expressed as
       n - ceil(n*sqrt(2)/2) = n - (floor(n*sqrt(2)/2)+1).  Since
       floor(n*sqrt(2)/2) = floor( sqrt(2*n*n) / 2 ), we compute that
       with integer arithmetic. */
    idx_t sqrt2nn = isqrt_idx((idx_t)2 * n * n);      /* floor( sqrt(2*n*n) ) */
    idx_t sqrt2n  = sqrt2nn / 2;                      /* floor( sqrt(2*n*n) / 2 ) */
    idx_t max_j   = n - sqrt2n - 1;                  /* assure non‑negative */

    seq_t total = 0;
    for (idx_t j = 0; j <= max_j; ++j) {
        idx_t inside = (idx_t)2 * j * n - j * j;
        idx_t r = isqrt_idx(inside);                 /* floor( sqrt(inside) ) */
        total += (seq_t)n - (seq_t)j - (seq_t)r;
    }
    return total;
}
