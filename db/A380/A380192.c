/*
 * A380192  Sum mod(10) of digits of n-th prime.
 * https://oeis.org/A380192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum of the nth prime modulo 10
 * Range:     u8 n <= 54, u16 n <= 7045, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0 as a safe default. */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t prime = (seq_t) get_prime(n);

    while (prime > 0)
    {
        sum += prime % 10;
        prime /= 10;
    }
    return sum % 10;
}
