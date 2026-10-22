/*
 * A353798  a(n) = 1 if n is either 4th power of a prime (A030514), or product
 *          of a prime and the square of a different prime (A054753),
 *          otherwise 0.
 * https://oeis.org/A353798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a prime's 4th power or the product of a prime
 *            and a different prime's square, otherwise 0
 * Range:     u8 n <= 63, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   factor_u64, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Check for prime fourth powers: n = p^4 */
    seq_t sqrt_n = floor_sqrt((seq_t)n);
    if (sqrt_n * sqrt_n == (seq_t)n) {
        seq_t root2 = floor_sqrt(sqrt_n);
        if (root2 * root2 == sqrt_n && is_prime((idx_t)root2)) {
            return 1;
        }
    }

    /* Check for product of a prime and a different prime's square: n = p * q^2, p != q */
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    if (cnt == 2) {
        if ((ex[0] == 1 && ex[1] == 2 && pr[0] != pr[1]) ||
            (ex[0] == 2 && ex[1] == 1 && pr[0] != pr[1])) {
            return 1;
        }
    }

    return 0;
}
