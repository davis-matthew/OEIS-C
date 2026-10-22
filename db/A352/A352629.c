/*
 * A352629  Number of ways to write n as a^2 + 2*b^2 + c^4 + 9*c^4 +
 *          3*c^2*d^2, where a,b,c,d are nonnegative integers.
 * https://oeis.org/A352629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of tuples (a,b,c,d) of non‑negative integers such
 *            that n = a^2 + 2*b^2 + c^4 + 9*d^4 + 3*c^2*d^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned values. */
static inline idx_t _int_sqrt_u64(idx_t v)
{
    idx_t x = v, y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + v / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Loop over c and d, compute term = c^4 + 9*d^4 + 3*c^2*d^2. */
    for (idx_t c = 0;; ++c) {
        idx_t c2 = c * c;
        idx_t c4 = c2 * c2;
        if (c4 > n)
            break;
        for (idx_t d = 0;; ++d) {
            idx_t d2 = d * d;
            idx_t d4 = d2 * d2;
            idx_t term = c4 + 9 * d4 + 3 * c2 * d2;
            if (term > n)
                break;
            idx_t remaining = n - term;

            /* Now count (a,b) with a^2 + 2*b^2 = remaining. */
            for (idx_t a_val = 0;; ++a_val) {
                idx_t a2 = a_val * a_val;
                if (a2 > remaining)
                    break;
                idx_t rem2 = remaining - a2;
                if (rem2 & 1)            /* odd, cannot be 2*b^2 */
                    continue;
                idx_t b2 = rem2 >> 1;
                idx_t b = _int_sqrt_u64(b2);
                if (b * b == b2)
                    ++count;
            }
        }
    }

    return count;
}
