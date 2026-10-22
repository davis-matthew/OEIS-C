/*
 * A340801  a(n) is the image of n under the map f defined as f(n) = n^2 - 2
 *          if n is an odd prime, f(n) = n/2 if n is even, and f(n) = n - 1
 *          otherwise.
 * https://oeis.org/A340801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) defined by f(n) = n^2 - 2 if n is an odd prime, n/2
 *            if n is even, n - 1 otherwise
 * Range:     u8 n <= 16, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Even numbers: divide by 2 */
    if ((n & 1) == 0) {
        return n / 2;
    }

    /* Odd numbers */
    if (is_prime(n)) {                /* odd prime */
        seq_t nn = (seq_t)n;
        return nn * nn - 2;
    } else {                           /* odd composite or 1 */
        return n - 1;
    }
}
