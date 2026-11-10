/*
 * A323011  a(n) = A172103(n) - A172104(n).
 * https://oeis.org/A323011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A172103(n) - A172104(n), where A172103(n) = #{k<=n : 6k-1
 *            prime} and A172104(n) = #{k<=n : 6k+1 prime}; computed by direct
 *            primality tests
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        if (is_prime(6 * k - 1))
            sum++;
        if (is_prime(6 * k + 1))
            sum--;
    }
    return sum;
}
