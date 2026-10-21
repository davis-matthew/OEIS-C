/*
 * A291436  The arithmetic function u(n,5,8).
 * https://oeis.org/A291436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   u(n,5,8) – 33 for n = 1,2,4; 27 for n = 3. For n ≥ 5: • If n is
 *            prime and n > 32, return 33. • Otherwise return the smallest
 *            divisor of n greater than 4 (which is n itself for primes ≤ 32)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)33;
    if (n == 2) return (seq_t)33;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)33;

    if (n > 32 && is_prime(n)) return (seq_t)33;

    for (idx_t i = 5; i <= n; ++i) {
        if (n % i == 0)
            return (seq_t)i;
    }
    /* n divides itself – this line is reached only for n <= 4
       which are handled above, but is kept for completeness.      */
    return (seq_t)n;
}
