/*
 * A259315  Nonprimes containing no zeros in decimal representation.
 * https://oeis.org/A259315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth nonprime number that has no zero digit in its
 *            decimal representation
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple primality test for seq_t values. Works for all seq_t sizes used in the problem. */
static bool is_prime_seq(seq_t n)
{
    if (n < (seq_t)2) return false;
    if (n % (seq_t)2 == (seq_t)0) return n == (seq_t)2;
    for (seq_t i = (seq_t)3; i * i <= n; i += (seq_t)2)
        if (n % i == (seq_t)0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for out-of-range calls */

    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)1;

    while (true)
    {
        /* Check for zero digits */
        seq_t temp = candidate;
        bool has_zero = false;
        while (temp > (seq_t)0) {
            if ((temp % (seq_t)10) == (seq_t)0) {
                has_zero = true;
                break;
            }
            temp /= (seq_t)10;
        }

        if (!has_zero) {
            if (candidate == (seq_t)1 || !is_prime_seq(candidate)) {
                count++;
                if (count == n) return candidate;
            }
        }
        candidate += (seq_t)1;
    }
}
