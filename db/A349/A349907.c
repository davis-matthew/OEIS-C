/*
 * A349907  Number of factorizations of n into odd factors > 1 (a(1) = 1 by
 *          convention).
 * https://oeis.org/A349907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is even, otherwise a(n) = the number of
 *            multiplicative partitions of the odd integer n (including n
 *            itself)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 8970 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t count_factorizations(uint64_t n, uint64_t min_factor)
{
    seq_t total = 0;
    /* enumerate odd divisors of n starting from min_factor */
    for (uint64_t f = min_factor; f <= n / f; f += 2) {
        if (n % f == 0) {
            total += count_factorizations(n / f, f);
        }
    }
    total += 1;               /* count the factorization consisting of n itself */
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* conventionally a(1)=1 */
    if ((n & 1) == 0) return 0;    /* even numbers have no odd factorizations */

    return count_factorizations((uint64_t)n, 3);
}
