/*
 * A306509  a(n) is the number of divisors of the sum of digits of n.
 * https://oeis.org/A306509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of the sum of decimal digits of n
 * Range:     u8 n <= 815, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e., a(n) = divisor_count(sum_of_digits(n))) */

#include "../utils/digit_sum.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute the sum of digits of n (n is an idx_t, but digit_sum expects seq_t) */
    seq_t sum_digits = digit_sum((seq_t)n);

    /* Count the positive divisors of the digit sum.
       Since sum_digits fits in uint64_t for any 64‑bit n, we can cast to
       uint64_t without loss. */
    return (seq_t)divisor_count_u64((uint64_t)sum_digits);
}
