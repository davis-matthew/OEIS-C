/*
 * A348146  a(n) = Sum_{d|n} (d!)^(n-d).
 * https://oeis.org/A348146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d∣n} (d!)^(n−d)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, factorial, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;                   /* offset guard */

    if (is_prime(n))                        /* primes give 2 */
        return 2;

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t term1 = big_pow(factorial(d1), (seq_t)(n - d1));
            sum += term1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = big_pow(factorial(d2), (seq_t)(n - d2));
                sum += term2;
            }
        }
    }
    return sum;
}
