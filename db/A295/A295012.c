/*
 * A295012  a(n) = sigma(12n - 1)/12, where sigma = sum of divisors (A000203).
 * https://oeis.org/A295012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(12n - 1)/12
 * Range:     u8 n <= 17, u16 n <= 3817, u32 n <= 9525, u64 n <= 9525,
 *            u128 n <= 9525, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t m = 12 * (seq_t)n - 1;
    return sigma_of((idx_t)m) / 12;
}
