/*
 * A245174  Second differences of A006450.
 * https://oeis.org/A245174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(n+2) - 2*b(n+1) + b(n), where b(n) = A006450(n) =
 *            prime(prime(n))
 * Range:     i8 n <= 135, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9882 (2026-11-07)
 * Helpers:   prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_nth.h"

static seq_t A006450_a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return (seq_t)prime_nth(prime_nth((uint64_t)n));
}

seq_t a(idx_t n)
{
    seq_t x = A006450_a(n);
    seq_t y = A006450_a(n + 1);
    seq_t z = A006450_a(n + 2);
    return z - y - y + x;
}
