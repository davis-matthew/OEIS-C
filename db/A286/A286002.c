/*
 * A286002  a(n) = 2n - d(n), where d(n) is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A286002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - d(n) (where d(n) is the number of divisors of n)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

static seq_t divisor_count_u64(uint64_t n)
{
    seq_t d = 1;          /* divisor count starts at 1 */
    uint64_t tmp = n;

    /* trial division by all possible prime factors */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
        }
    }

    /* if anything is left, it is a prime > sqrt(original n) */
    if (tmp > 1) {
        d *= (seq_t)2;
    }

    return d;
}

seq_t a(idx_t n)
{
    /* n is >= 1 by definition of the sequence */
    seq_t n_seq = (seq_t)n;
    seq_t d = divisor_count_u64(n);
    return n_seq + n_seq - d;   /* a(n) = 2*n - d(n) */
}
