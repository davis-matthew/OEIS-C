/*
 * A398781  If k appears, 6*k does not.
 * https://oeis.org/A398781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth positive integer k such that min(v_2(k), v_3(k)) is
 *            even (i.e., the exponent of the highest power of 6 dividing k is
 *            even)
 * Range:     u8 n <= 219, u16 n <= 8571, u32 n <= 8571, u64 n <= 8571,
 *            u128 n <= 8571, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t v2 = 0, v3 = 0, t;

        /* exponent of 2 */
        t = k;
        while ((t & 1) == 0) {
            ++v2;
            t >>= 1;
        }

        /* exponent of 3 */
        t = k;
        while (t % 3 == 0) {
            ++v3;
            t /= 3;
        }

        /* min exponent of 2 or 3 */
        idx_t m = (v2 < v3) ? v2 : v3;
        if ((m & 1) == 0) {          /* even */
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
