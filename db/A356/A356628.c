/*
 * A356628  a(n) = n! * Sum_{k=0..floor(n/2)} (n - 2*k)^k/(n - 2*k)!.
 * https://oeis.org/A356628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n - 2*k)^k/(n - 2*k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   int_pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_pow_uint.h"

seq_t a(idx_t n) {
    idx_t m = n / 2;           /* floor(n/2) */
    seq_t sum   = 0;
    seq_t ratio = 1;           /* ratio = n! / (n-2k)! starts with k=0 */
    idx_t d = n;               /* current value of n-2k */

    for (idx_t k = 0; k <= m; ++k) {
        seq_t powval = int_pow_uint(d, k);          /* d^k */
        sum += ratio * powval;                      /* term contribution */

        if (k < m) {                              /* prepare for next k */
            ratio = ratio * (seq_t)d * (seq_t)(d - 1);
            d -= 2;
        }
    }
    return sum;
}
