/*
 * A354032  a(n) = 1 if phi(n)+sigma(n) == 2 (mod 4), otherwise 0.
 * https://oeis.org/A354032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if phi(n)+sigma(n) == 2 (mod 4), otherwise 0
 * Range:     u8 n <= 8647, u16 n <= 9955, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include <stdint.h>

/* Compute the sum of the positive divisors of n.
   This function works for n up to 2^64 and uses idx_t for the loop
   variable, avoiding overflow by testing i <= n/i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t phi_n   = phi(n);
    seq_t sigma_n = sigma_of(n);
    seq_t mod4    = (phi_n + sigma_n) % 4;
    return (mod4 == 2) ? 1 : 0;
}
