/*
 * A275676  Number of ordered ways to write n as 4^k*(1+5*x^2+y^2) + z^2,
 *          where k,x,y,z are nonnegative integers with x <= y.
 * https://oeis.org/A275676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of tuples (k,x,y,z) of nonnegative integers with x
 *            <= y such that n = 4^k * (1 + 5*x^2 + y^2) + z^2. Algorithm:
 *            brute‑force over k (powers of 4) and z^2, then for each residual
 *            count the representations of B = (n - z^2)/4^k – 1 as 5x^2 + y^2
 *            with x <= y
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned integers. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v, best = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index; assume n >= 1. */
    seq_t result = 0;
    idx_t pow4 = 1;                 /* 4^k */

    while (pow4 <= n) {             /* loop over k */
        idx_t z = 0;
        while (z * z <= n) {        /* loop over z */
            idx_t rem = n - z * z;  /* remaining after subtracting z^2 */
            if (rem % pow4 == 0) {
                idx_t A = rem / pow4;   /* 1 + 5*x^2 + y^2 */
                if (A > 0) {
                    idx_t B = A - 1;     /* 5*x^2 + y^2 */
                    /* count (x,y) with x <= y and 5*x^2 + y^2 = B */
                    idx_t max_x = isqrt_u64(B / 5);
                    idx_t reps = 0;
                    for (idx_t x = 0; x <= max_x; ++x) {
                        idx_t val = B - 5 * x * x;
                        if (val < 0) continue;
                        idx_t y = isqrt_u64(val);
                        if (y * y == val && y >= x) {
                            ++reps;
                        }
                    }
                    result += (seq_t)reps;
                }
            }
            ++z;
        }
        if (pow4 > n / 4) break;   /* prevent overflow for next power of 4 */
        pow4 *= 4;
    }
    return result;
}
