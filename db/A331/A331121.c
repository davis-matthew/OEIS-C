/*
 * A331121  a(n) is the smallest positive integer k for which tau(k) does not
 *          divide sigma(n).
 * https://oeis.org/A331121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k >= 1 such that tau(k) does not divide sigma(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include <stdbool.h>
#include <stdint.h>

/* Compute the number of divisors tau(k) by trial division.
   The result fits in seq_t. */
static seq_t tau_of(idx_t k)
{
    idx_t x = k;
    seq_t tau = 1;

    /* handle factor 2 */
    if ((x & 1) == 0) {
        idx_t exp = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            ++exp;
        }
        tau *= (seq_t)(exp + 1);
    }

    /* odd factors */
    for (idx_t p = 3; p <= x / p; p += 2u) {
        if (x % p == 0) {
            idx_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (x > 1) {
        tau *= (seq_t)2;
    }
    return tau;
}

/* Return a(n) as defined by OEIS A331121. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* undefined for n < 1; harmless guard */
    }

    seq_t sigma = sigma_of(n);

    for (idx_t k = 1; ; ++k) {
        seq_t tau = tau_of(k);
        if (sigma % tau != (seq_t)0) {
            return (seq_t)k;
        }
    }
}
