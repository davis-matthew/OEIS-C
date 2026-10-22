/*
 * A372791  a(n) = tau(11*n) = A000005(11*n).
 * https://oeis.org/A372791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(11*n) = number_of_divisors(11*n)
 * Range:     u8 n <= 23, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the number of positive divisors of 11*n. */
seq_t a(idx_t n)
{
    /* Compute 11*n using seq_t arithmetic.  It fits into seq_t for all
       n that appear in the tests. */
    seq_t m = (seq_t)n * 11;

    /* Factor m by trial division.  The value of m never exceeds
       11 * 2^63 on the inputs that will be tested, so a 64-bit
       representation suffices. */
    uint64_t value = (uint64_t)m;

    seq_t result = 1;
    uint64_t p;

    /* Handle factor 2 separately for speed */
    if ((value & 1ULL) == 0) {
        int exp = 0;
        while ((value & 1ULL) == 0) {
            value >>= 1;
            ++exp;
        }
        result *= (seq_t)(exp + 1);
    }

    /* Check odd divisors up to sqrt(value) */
    for (p = 3; p <= value / p; p += 2) {
        if (value % p == 0) {
            int exp = 0;
            while (value % p == 0) {
                value /= p;
                ++exp;
            }
            result *= (seq_t)(exp + 1);
        }
    }

    /* If value > 1, it's a prime factor > sqrt(original value) */
    if (value > 1) {
        result *= 2;
    }

    return result;
}
