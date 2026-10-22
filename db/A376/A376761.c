/*
 * A376761  Number of primes between the n-th composite number c(n) and
 *          2*c(n).
 * https://oeis.org/A376761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(2*A002808(n)) - A002808(n) + n + 1
 * Range:     u8 n <= 1739, u16 n <= 9362, u32 n <= 9362, u64 n <= 9362,
 *            u128 n <= 9362, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find the n-th composite number c = A002808(n) */
    idx_t count = 0;
    idx_t c = 0;
    idx_t num = 4;                     /* composites start at 4 */
    while (1) {
        if (!is_prime(num)) {          /* num is composite (>=4) */
            ++count;
            if (count == n) {
                c = num;
                break;
            }
        }
        ++num;
    }

    /* Count primes strictly between c and 2*c */
    seq_t primes_between = 0;
    for (idx_t p = c + 1; p < 2 * c; ++p) {
        if (is_prime(p))
            ++primes_between;
    }

    return primes_between;
}
