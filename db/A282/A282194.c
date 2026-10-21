/*
 * A282194  a(n) = smallest positive k such that 2*n + 2^k + 1 is composite.
 * https://oeis.org/A282194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 935, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A282194: a(n) = smallest positive k such that 2*n + 2^k + 1 is composite */
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t two_n = ((seq_t)n) * 2;
    for (idx_t k = 1; ; ++k) {
        seq_t val = two_n + pow2(k) + 1;
        if (!is_prime_big(val))
            return (seq_t)k;
    }
}
