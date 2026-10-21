/*
 * A253275  a(n) = Sum_{i=1..floor(n/2)} d( i*(n-i) ), where d(n) = the number
 *          of divisors of n.
 * https://oeis.org/A253275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} d(i*(n-i)) where d(x) is the number
 *            of divisors of x
 * Range:     u8 n <= 40, u16 n <= 2686, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 there are no terms in the sum. */
    if (n <= 1)
        return (seq_t)0;

    idx_t half = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= half; ++i) {
        /* Compute i*(n-i) as 64‑bit to avoid overflow of idx_t. */
        uint64_t prod = (uint64_t)i * ((uint64_t)n - i);

        /* divisor_count_u64 returns the number of divisors of a 64‑bit integer. */
        seq_t d = (seq_t)divisor_count_u64(prod);

        sum += d;
    }
    return sum;
}
