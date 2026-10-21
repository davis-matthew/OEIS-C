/*
 * A283237  a(n) = sigma_2(3*n).
 * https://oeis.org/A283237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_k(3*n, 2) (sum of squares of the divisors of 3*n)
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

/* Return the n-th term of OEIS sequence A283237. */
seq_t a(idx_t n)
{
    /* Compute m = 3*n.  The multiplication is safe for all practical n,
       since n is an unsigned 64‑bit integer and 3*n fits easily in the type. */
    idx_t m = n * 3;

    /* The sigma_k function from utils returns σ_k(m) = sum_{d|m} d^k.
       Here we need σ_2(m), i.e., the sum of squares of the divisors. */
    return sigma_k(m, 2);
}
