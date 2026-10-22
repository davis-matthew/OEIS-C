/*
 * A327420  Building sums recursively with the divisibility properties of
 *          their partial sums.
 * https://oeis.org/A327420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(0) = 1
 *   For n > 0: let s = n + 1.
 *   For k from n+1 down to 2:
 *     if s % k == 0:
 *        q = s / k
 *        if q is even: s += k
 *        else:        s -= k
 *   a(n) = s
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t s = (seq_t)n + 1;
    for (idx_t k = n + 1; k >= 2; --k) {
        if (s % k == 0) {
            seq_t q = s / k;
            if ((q & (seq_t)1) == 0) {
                s += k;       /* q even, add k */
            } else {
                s -= k;       /* q odd, subtract k */
            }
        }
    }
    return s;
}
