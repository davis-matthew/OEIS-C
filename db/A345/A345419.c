/*
 * A345419  Row 2 of array in A345417.
 * https://oeis.org/A345419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (p+1)/3 if p == 2 (mod 3), -(p-1)/3 if p == 1 (mod 3),
 *            a(2) = 0, where p = prime(n) (the minimal u in 3u + p v =
 *            gcd(3,p))
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    if (p == 3) return (seq_t)0;
    if (p % 3 == 2) return (seq_t)((p + 1) / 3);
    return -(seq_t)((p - 1) / 3);
}
