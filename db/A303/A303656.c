/*
 * A303656  Number of ways to write n as a^2 + b^2 + 3^c + 5^d, where a,b,c,d
 *          are nonnegative integers with a <= b.
 * https://oeis.org/A303656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = a^2 + b^2 + 3^c + 5^d with
 *            a,b,c,d >= 0 and a <= b
 * Range:     u8 n <= 8975, u16 n <= 8975, u32 n <= 8975, u64 n <= 8975,
 *            u128 n <= 8975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values */
static inline unsigned long long isqrt_idx(unsigned long long x)
{
    unsigned long long lo = 0, hi = x;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {        /* mid == 0 always satisfies mid^2 <= x */
            if (hi == 0) return 0;
            lo = 1;
            continue;
        }
        if (mid <= x / mid) {  /* mid is not too large */
            unsigned long long nxt = mid + 1;
            if (nxt > x / nxt) return mid;  /* mid is the floor sqrt */
            lo = nxt;
        } else {
            hi = mid - 1;
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* iterate over powers of 3 */
    for (idx_t pow3 = 1; pow3 <= n; pow3 *= 3) {
        if (pow3 == 0) break;                      /* overflow guard   */
        /* iterate over powers of 5 */
        for (idx_t pow5 = 1; pow5 <= n; pow5 *= 5) {
            if (pow5 == 0) break;                  /* overflow guard   */
            if (pow3 > n - pow5) break;            /* no further pow5  */
            idx_t rem = n - pow3 - pow5;           /* remaining sum   */

            idx_t sqrt_rem = isqrt_idx(rem);
            for (idx_t a = 0; a <= sqrt_rem; ++a) {
                idx_t a2   = a * a;
                idx_t rem2 = rem - a2;             /* 0 <= rem2 <= rem */
                idx_t b    = isqrt_idx(rem2);
                idx_t b2   = b * b;
                if (b2 == rem2 && a <= b) {
                    ++count;
                }
            }
            if (pow5 > n / 5) break;
        }
        if (pow3 > n / 3) break;
    }

    return count;
}
