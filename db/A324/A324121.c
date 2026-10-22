/*
 * A324121  a(n) = gcd(n*d(n), sigma(n)), where d(n) = number of divisors of n
 *          (A000005) and sigma(n) = sum of divisors of n (A000203).
 * https://oeis.org/A324121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n*d(n), sigma(n))
 * Range:     u8 n <= 39, u16 n <= 5081, u32 n <= 9603, u64 n <= 9603,
 *            u128 n <= 9603, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   d(n) = number of divisors of n (A000005)               */
/*   sigma(n) = sum of divisors of n (A000203)              */

#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* n is at least 1 (OEIS offset 1) */
    seq_t nd = (seq_t)n * (seq_t)divisor_count_u64(n);
    seq_t sig = sigma_of(n);
    return gcd(nd, sig);
}
