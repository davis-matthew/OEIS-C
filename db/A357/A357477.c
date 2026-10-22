/*
 * A357477  a(n) is the smallest k such that the square root of k*n rounds to
 *          a prime.
 * https://oeis.org/A357477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A357477: a(n) = smallest k such that
 * round(sqrt(k*n)) is prime.  The rounding is performed by
 * choosing the nearest integer; ties are rounded upward. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is assumed to be >= 1.  k starts at 1 and is incremented
     * until a suitable prime is found.  For each k we compute
     * m = round(sqrt(k*n)) and test if m is prime. */
    idx_t k = 1;
    while (1) {
        seq_t prod = (seq_t)n * (seq_t)k;         // k * n
        seq_t r = floor_sqrt(prod);              // floor(sqrt(prod))
        seq_t r2 = r * r;
        seq_t r1 = (r + 1) * (r + 1);

        /* Determine nearest integer to sqrt(prod). */
        seq_t diff_low  = prod - r2;
        seq_t diff_high = r1 - prod;
        seq_t m = r;
        if (diff_low > diff_high || diff_low == diff_high) {
            m = r + 1;
        }

        if (is_prime_big(m)) {
            return (seq_t)k;
        }
        ++k;
    }
}
