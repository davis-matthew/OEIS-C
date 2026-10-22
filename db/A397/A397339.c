/*
 * A397339  Number of distinct triples (i,p,f) arising from a simple graph
 *          with n edges, where i, p and f count the edges having 0, 1, and 2
 *          endpoints of degree greater than 1, respectively.
 * https://oeis.org/A397339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - 5 for n >= 3; a(1) = 1, a(2) = 2
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    seq_t N = (seq_t)n;
    seq_t result = (N + 2) * (N + 1) / 2;
    result -= 5;
    return result;
}
