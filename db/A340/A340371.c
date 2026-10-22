/*
 * A340371  a(n) = 1 if the odd part of n is noncomposite, 0 otherwise.
 * https://oeis.org/A340371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is noncomposite (1 or an odd
 *            prime), 0 otherwise
 * Range:     u8 n <= 2253, u16 n <= 7630, u32 n <= 7630, u64 n <= 7630,
 *            u128 n <= 7630, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* not part of OEIS offset, but safe */
    uint64_t x = (uint64_t)n;
    /* strip powers of 2 to get the odd part */
    while ((x & 1ULL) == 0ULL) x >>= 1ULL;
    if (x == 1ULL) return (seq_t)1;
    return (seq_t)( is_prime_big((seq_t)x) ? 1 : 0 );
}
