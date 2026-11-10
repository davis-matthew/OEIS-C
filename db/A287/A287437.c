/*
 * A287437  Positions of 2 in A053838.
 * https://oeis.org/A287437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n - A053838(n-1), where A053838(m) = (base-3 digit sum
 *            of m) mod 3
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9515 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A053838_a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += (seq_t)(n % 3);
        n /= 3;
    }
    return sum % 3;
}

seq_t a(idx_t n)
{
    return (seq_t)3 * (seq_t)n - A053838_a(n - 1);
}
