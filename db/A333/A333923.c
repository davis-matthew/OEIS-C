/*
 * A333923  a(n) is the smallest positive integer such that n^a(n) is
 *          divisible by n+a(n).
 * https://oeis.org/A333923
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer a such that n^a is divisible by
 *            n+a
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* any value is acceptable for indices below the offset */
        return 0;

    if (is_prime(n)) {                /* prime case: a(n) = n*(n-1) */
        seq_t sn = (seq_t)n;
        return sn * (sn - 1);
    }

    seq_t sn = (seq_t)n;
    for (idx_t a_val = 1; ; ++a_val) { /* test successive a values */
        seq_t m = sn + (seq_t)a_val;   /* m = n + a */
        if (mod_pow(sn, a_val, m) == 0)
            return (seq_t)a_val;
    }
}
