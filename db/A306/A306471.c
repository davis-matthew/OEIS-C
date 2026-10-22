/*
 * A306471  Number of ways to write n as C(2w+1,2) + C(x+2,3) + C(y+3,4) +
 *          C(z+4,5) with w,x,y,z nonnegative integers, where C(n,k) denotes
 *          the binomial coefficient n!/(k!*(n-k)!).
 * https://oeis.org/A306471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z) of non‑negative integers
 *            satisfying n = C(2w+1,2) + C(x+2,3) + C(y+3,4) + C(z+4,5). This
 *            implementation counts all solutions by brute‑force search
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t w, x, y, z;
    seq_t A, B, C, rem, val;

    for (w = 0; ; ++w) {
        A = (seq_t)w * (((seq_t)2 * w) + 1);        /* w*(2w+1) */
        if (A > n) break;

        for (x = 0; ; ++x) {
            B = (seq_t)x * (x + 1) * (x + 2) / 6;   /* C(x+2,3) */
            if (A + B > n) break;

            for (y = 0; ; ++y) {
                C = (seq_t)y * (y + 1) * (y + 2) * (y + 3) / 24; /* C(y+3,4) */
                if (A + B + C > n) break;

                rem = n - A - B - C;

                /* search for z such that C(z+4,5) == rem */
                for (z = 0; ; ++z) {
                    val = (seq_t)z * (z + 1) * (z + 2) * (z + 3) * (z + 4) / 120; /* C(z+4,5) */
                    if (val == rem) count++;
                    if (val > rem) break;
                }
            }
        }
    }
    return count;
}
