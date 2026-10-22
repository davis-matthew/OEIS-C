/*
 * A378898  a(n) is the least k > 0 such that (n+k)^2 + n^2 is prime.
 * https://oeis.org/A378898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k>0 : (n+k)^2 + n^2 is prime}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute the least positive k such that (n+k)^2 + n^2 is prime. */
seq_t a(idx_t n)
{
    /* n is at least 1 (by the definition of the sequence) */
    idx_t n_val = n;
    idx_t n2 = n_val * n_val;

    for (idx_t k = 1; ; ++k) {
        idx_t m  = n_val + k;           /* m = n + k */
        idx_t val = m * m + n2;         /* (n+k)^2 + n^2 */

        /* Test primality of val (fits in idx_t for the problem constraints). */
        if (is_prime(val))
            return (seq_t)k;
    }
}
