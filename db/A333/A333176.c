/*
 * A333176  a(n) = Sum_{k=1..n} (binomial(n,k) mod 2) * prime(k).
 * https://oeis.org/A333176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..n} (binomial(n,k) mod 2) * prime(k)
 * Range:     u8 n <= 14, u16 n <= 190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)                /* Undefined for n < 1 – return 0 for safety */
        return 0;

    seq_t sum = 0;             /* Accumulator in seq_t for large values */

    /* For each k, binomial(n,k) is odd iff (k & (n - k)) == 0 */
    for (idx_t k = 1; k <= n; ++k) {
        if ((k & (n - k)) == 0)            /* binomial(n,k) is odd */
            sum += (seq_t)get_prime(k);    /* prime(k) */
    }

    return sum;
}
