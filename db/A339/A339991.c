/*
 * A339991  The number of steps that n requires to reach 1 under the map: m ->
 *          m/2 if m is even, m-> m^2 - 1 if m is an odd prime, otherwise m ->
 *          m - 1. a(n) = -1 if 1 is never reached.
 * https://oeis.org/A339991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 1 under the map * m -> m/2 if m
 *            is even, * -> m^2 - 1 if m is an odd prime, * -> m - 1
 *            otherwise. * The function returns the step count (seq_t) for n ≥
 *            1. * If 1 is never reached the function would return -1
 *            (conjectured * never to happen). This implementation assu...
 * Range:     u8 n <= 16, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1)
        return 0;           /* a(1) = 0 */

    seq_t m = (seq_t)n;     /* current value */
    seq_t steps = 0;

    while (m != (seq_t)1) {
        if ((m & (seq_t)1) == (seq_t)0) {    /* even */
            m = m / (seq_t)2;
        } else {
            /* odd – check primality */
            uint64_t m64 = (uint64_t)m;     /* safe for n up to ~10^4 */
            if (is_prime(m64)) {
                m = m * m - (seq_t)1;
            } else {
                m = m - (seq_t)1;
            }
        }
        steps++;
    }
    return steps;
}
