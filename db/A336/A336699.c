/*
 * A336699  a(n) = A000265(1+A000265(sigma(A000265(n)))), where A000265(k)
 *          gives the odd part of k, and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A336699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A000265(1+A000265(sigma(A000265(n))))
 * Range:     u8 n <= 439, u16 n <= 9626, u32 n <= 9626, u64 n <= 9626,
 *            u128 n <= 9626, big any n
 * Helpers:   odd_part, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = odd_part(1 + odd_part(sigma(odd_part(n)))) */

#include "../utils/odd_part.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* odd part of n */
    seq_t odd_n  = odd_part((seq_t)n);

    /* sigma of that odd part (requires an idx_t argument) */
    idx_t odd_n_idx = (idx_t)odd_n;
    seq_t sigma_val = sigma_of(odd_n_idx);

    /* odd part of sigma */
    seq_t odd_sigma = odd_part(sigma_val);

    /* final odd part of 1 + odd_sigma */
    seq_t result = odd_part((seq_t)1 + odd_sigma);

    return result;
}
