/*
 * A265672  a(n) = n + floor((n+1)/7)*(-1)^((n+1) mod 7).
 * https://oeis.org/A265672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n+1)/7) * (-1)^((n+1) mod 7)
 * Range:     u8 n <= 224, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute k = floor((n+1)/7) and its parity term */
    idx_t n_plus1 = n + 1;
    seq_t k = (seq_t)(n_plus1 / 7);      /* floor((n+1)/7) */
    idx_t rem = n_plus1 % 7;             /* (n+1) mod 7 */

    seq_t res = n;                       /* start with n */
    if ((rem & 1) == 0) {                /* even remainder => sign +1 */
        res += k;
    } else {                             /* odd remainder => sign -1 */
        res -= k;
    }
    return res;
}
