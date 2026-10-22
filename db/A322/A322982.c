/*
 * A322982  If n is a noncomposite, then a(n) = 2*n - 1, otherwise a(n) =
 *          A032742(n), the largest proper divisor of n.
 * https://oeis.org/A322982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n-1 if n is 1 or a prime, otherwise a(n) = n /
 *            smallest_prime_factor(n) (the largest proper divisor)
 * Range:     u8 n <= 130, u16 n <= 9957, u32 n <= 9957, u64 n <= 9957,
 *            u128 n <= 9957, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 1 is treated as non‑composite (so a(1)=1) */
    if (n == 1)
        return (seq_t)1;

    /* For primes: a(n) = 2*n - 1 */
    if (is_prime(n))
        return ((seq_t)n * 2) - 1;

    /* Composite case: return the largest proper divisor,
       which is n divided by its smallest prime factor.   */
    idx_t spf = 0;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }
    /* spf should always be found for composite n; guard as a fallback */
    if (spf == 0)
        return (seq_t)1;          /* should never happen */

    return (seq_t)n / (seq_t)spf;
}
