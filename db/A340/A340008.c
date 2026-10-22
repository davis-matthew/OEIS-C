/*
 * A340008  a(n) is the image of n under the map: n -> n/2 if n is even, n->
 *          n^2 - 1 if n is an odd prime, otherwise n -> n - 1.
 * https://oeis.org/A340008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A340008: a(n) = n/2 if n is even; a(n) = n^2-1 if n is an odd prime; otherwise a(n) = n-1. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {            /* n is even */
        return (seq_t)n / 2;
    }
    if (is_prime(n)) {             /* n is an odd prime */
        seq_t nn = (seq_t)n;
        return nn * nn - 1;
    }
    return (seq_t)n - 1;           /* odd composite or n == 1 */
}
