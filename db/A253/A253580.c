/*
 * A253580  A fractal tree, read by rows: for n > 1: T(n,0) = T(n-1,0)+2,
 *          T(n,2*n) = T(n-1,0)+3, and for k=1..2*n-1: T(n,k) = T(n-1,k-1).
 * https://oeis.org/A253580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, nice, look
 * Formula:   a(n) is obtained by locating n in the triangular array. * Let r
 *            = ⌊√n⌋ and k = n – r². Then * a(n) = 2*(r – k) – 1 if k < r, *
 *            a(n) = 0 if k == r, * a(n) = 2*(k – r) if k > r
 * Range:     u8 n <= 9931, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit indices. */
static inline idx_t isqrt_idx(idx_t x)
{
    if (x < 2)
        return x;
    idx_t low = 1;
    idx_t high = (idx_t)1 << 32;           /* 2^32 > sqrt(2^64-1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                           /* high² ≤ x < (high+1)² */
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_idx(n);           /* floor(sqrt(n)) */
    idx_t k = n - r * r;
    if (k < r) {
        seq_t diff = (seq_t)(r - k);
        return 2 * diff - 1;
    } else if (k == r) {
        return 0;
    } else { /* k > r */
        seq_t diff = (seq_t)(k - r);
        return 2 * diff;
    }
}
