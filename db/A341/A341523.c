/*
 * A341523  Number of prime factors (with multiplicity) shared by n and
 *          sigma(n): a(n) = bigomega(gcd(n, sigma(n))).
 * https://oeis.org/A341523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(gcd(n, sigma(n)))
 * Range:     u8 n <= 912, u16 n <= 9835, u32 n <= 9835, u64 n <= 9835,
 *            u128 n <= 9835, big any n
 * Helpers:   bigomega, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t g = gcd((seq_t)n, sigma);
    return bigomega((idx_t)g);
}
