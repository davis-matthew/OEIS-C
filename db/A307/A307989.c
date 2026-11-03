/*
 * A307989  a(n) = n - pi(2*n) + pi(n-1), where pi is the prime counting
 *          function.
 * https://oeis.org/A307989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - pi(2*n) + pi(n-1)
 * Range:     u8 n <= 304, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 8902 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the OEIS offset */
    seq_t pi2 = prime_pi(n * 2);
    seq_t pin1 = (n > 1) ? prime_pi(n - 1) : 0;
    return (seq_t)n - pi2 + pin1;
}
