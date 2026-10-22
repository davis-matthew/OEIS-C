/*
 * A353811  a(n) = 1 if sigma(n) is odd, and has an odd number of prime
 *          factors (with multiplicity), otherwise 0. Here sigma is the sum of
 *          divisors function.
 * https://oeis.org/A353811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) is odd and sigma(n) has an odd * number of
 *            prime factors (with multiplicity), otherwise 0 * (off‑by‑offset:
 *            a(1) corresponds to n = 1)
 * Range:     u8 n <= 9648, u16 n <= 9648, u32 n <= 9648, u64 n <= 9648,
 *            u128 n <= 9648, big any n
 * Helpers:   count_prime_factors, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n < 1u) {
        return 0;                /* undefined for n = 0 in OEIS */
    }

    /* Sigma of n */
    seq_t sig = sigma_of(n);

    /* Check that sigma(n) is odd */
    if ((sig % 2u) == 0u) {
        return 0;
    }

    /* Number of prime factors (with multiplicity) of sigma(n) */
    seq_t omega = count_prime_factors(sig);

    /* Return 1 iff this count is odd */
    return (omega % 2u) ? 1 : 0;
}
