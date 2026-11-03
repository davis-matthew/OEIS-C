/*
 * A256885  a(n) = n*(n + 1)/2 - pi(n), where pi(n) = A000720(n) is the prime
 *          counting function.
 * https://oeis.org/A256885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - pi(n); where pi(n) is the prime counting
 *            function
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9502 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; 0 is undefined but return 0 for safety */
    seq_t nn  = (seq_t)n;
    seq_t tri = nn * (nn + 1) / 2;
    seq_t pi  = prime_pi(n);
    return tri - pi;
}
