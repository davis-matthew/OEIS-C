/*
 * A294905  Characteristic function for A000120-nonabundant numbers: a(n) = 1
 *          if A292257(n) <= A000120(n), and 0 otherwise.
 * https://oeis.org/A294905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the total number of 1's in binary representations *
 *            of all proper divisors of n (A292257(n)) is <= the binary weight
 *            of n * (A000120(n)); otherwise 0. This implements a(n) =
 *            [A292257(n) <= * A000120(n)] with a straightforward divisor
 *            enumeration
 * Range:     u8 n <= 9799, u16 n <= 9799, u32 n <= 9799, u64 n <= 9799,
 *            u128 n <= 9799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Sum of popcounts of proper divisors */
    idx_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i < n) {
                sum += (idx_t)__builtin_popcountll((uint64_t)i);
            }
            idx_t j = n / i;
            if (j != i && j < n) {
                sum += (idx_t)__builtin_popcountll((uint64_t)j);
            }
        }
    }
    idx_t popcnt_n = (idx_t)__builtin_popcountll((uint64_t)n);
    return (seq_t)(sum <= popcnt_n);
}
