/*
 * A312865  Coordination sequence Gal.5.91.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊9*n/2⌋ + f(n) where the correction term f(n) depends on
 *            n mod 6: - f(0) = 1 - f(n) for n % 6 == 0 (n>0) = -(n/6) - f(n)
 *            for n % 6 == 1 = (n-1)/6 - f(n) for n % 6 == 2 = 0 - f(n) for n
 *            % 6 == 3 = -(n-1)/2 - f(n) for n % 6 == 4 = 0 - f(n) for n % 6
 *            == 5 = ((n-5)/6)+2 This piecewise linear f...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute floor(9*n/2) using signed 64‑bit arithmetic to avoid overflow
       on intermediate results, then apply the appropriate correction term. */
    int64_t base = ((int64_t)n * 9) / 2;          /* ⌊9*n/2⌋  */
    int64_t f = 0;

    if (n == 0) {
        f = 1;
    } else {
        switch (n % 6) {
        case 0:   /* n = 6,12,18,… */
            f = -((int64_t)n / 6);
            break;
        case 1:   /* n = 1,7,13,… */
            f = ((int64_t)n - 1) / 6;
            break;
        case 2:   /* n = 2,8,14,… */
            f = 0;
            break;
        case 3:   /* n = 3,9,15,… */
            f = -(((int64_t)n - 1) / 2);
            break;
        case 4:   /* n = 4,10,16,… */
            f = 0;
            break;
        case 5:   /* n = 5,11,17,… */
            f = ((int64_t)n - 5) / 6 + 2;
            break;
        }
    }

    return (seq_t)(base + f);
}
