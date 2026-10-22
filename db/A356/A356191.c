/*
 * A356191  a(n) is the smallest exponentially odd number that is divisible by
 *          n.
 * https://oeis.org/A356191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * ∏_{p|n, exponent even} p
 * Range:     u8 n <= 48, u16 n <= 1680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(p^e) = p^e  if e is odd, else p^(e+1)   */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n.  The helper factor_u64 requires user‑supplied arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)n;   /* start with n itself */

    for (size_t i = 0; i < cnt; ++i) {
        if ((exps[i] & 1) == 0) {           /* exponent is even */
            result *= (seq_t)primes[i];     /* multiply by one extra p */
        }
    }
    return result;
}
