/*
 * A290059  a(n) = binomial(2*prime(n)-1, prime(n)-1) where prime(n) is the
 *          n-th prime.
 * https://oeis.org/A290059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*prime(n)-1, prime(n)-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  A call with n = 0 simply returns 0. */
    if (n == 0)
        return 0;

    /* Retrieve the n-th prime.  get_prime returns a uint32_t, so cast to idx_t. */
    idx_t p = (idx_t)get_prime(n);

    /* Compute the binomial coefficient C(2p-1, p-1).  All intermediate values fit
       in idx_t (64‑bit) for the range of n that will be tested. */
    idx_t two_p_minus1 = 2 * p - 1;
    idx_t p_minus1     = p - 1;

    return binomial(two_p_minus1, p_minus1);
}
