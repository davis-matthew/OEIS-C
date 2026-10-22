/*
 * A303637  Number of ways to write n as x^2 + y^2 + 2^z + 5*2^w, where
 *          x,y,z,w are nonnegative integers with x <= y.
 * https://oeis.org/A303637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of representations of n as
   x^2 + y^2 + 2^z + 5 * 2^w, with x ≤ y and all variables
   non‑negative integers.  This follows the OEIS description
   of sequence A303637.  The implementation uses only integer
   arithmetic, avoiding any external dependencies except the
   standard headers.  Integer square roots are computed by a
   binary search that stays within 64‑bit limits.  The routine
   works for all n ≥ 1 and is efficient enough for n up to
   at least 10⁴. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)).  Works for all n in
   the range of idx_t (unsigned 64‑bit). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t left = 0, right = n, ans = 0;
    while (left <= right) {
        idx_t mid = left + ((right - left) >> 1);
        if (mid > 0 && mid > n / mid)   /* avoid overflow mid*mid > n */
            right = mid - 1;
        else {
            ans = mid;
            left = mid + 1;
        }
    }
    return ans;
}

/* Count solutions to x^2 + y^2 = m with x ≤ y, x,y ≥ 0. */
static inline seq_t count_two_square(idx_t m)
{
    seq_t cnt = 0;
    const idx_t maxx = isqrt_u64(m);
    for (idx_t x = 0; x <= maxx; ++x) {
        const idx_t x2 = x * x;
        const idx_t y2 = m - x2;
        const idx_t y  = isqrt_u64(y2);
        if (y * y == y2 && x <= y)
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 6)   /* All terms before n = 6 are zero by definition */
        return 0;

    seq_t total = 0;

    /* Enumerate all powers of 2 up to n. */
    idx_t p2 = 1;
    while (p2 <= n) {
        /* Enumerate all powers of 5*2^w up to n. */
        idx_t p5 = 5;
        while (p5 <= n) {
            const idx_t sum = p2 + p5;
            if (sum > n) {
                /* Since p5 is increasing, no larger p5 will work */
                break;
            }
            const idx_t m = n - sum;
            total += count_two_square(m);
            /* Prepare next power of 5*2^w (double it) */
            if (p5 > (n >> 1))
                break;   /* avoid overflow */
            p5 <<= 1;
        }

        /* Prepare next power of 2 (double it) */
        if (p2 > (n >> 1))
            break;       /* avoid overflow */
        p2 <<= 1;
    }

    return total;
}
