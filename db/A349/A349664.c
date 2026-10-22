/*
 * A349664  a(n) is the number of solutions for n^4 = z^2 - x^2 with {z,x} >=
 *          1.
 * https://oeis.org/A349664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of n^4 = z^2 - x^2 (z,x ≥ 1). For odd
 *            n: a = (∏ (4e_i+1) - 1)/2 where n = ∏ p_i^{e_i} (odd primes).
 *            For even n: let 2^k || n. Then a = ((4k-1) * ∏ (4e_i+1) - 1)/2
 *            with e_i exponents of odd primes in n
 * Range:     u8 n <= 419, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_factor_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/odd_factor_product.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety; defined only for n ≥ 1 */
    /* Determine exponent of 2 in n */
    uint64_t k = 0;
    uint64_t temp = (uint64_t)n;
    while ((temp & 1ULL) == 0ULL) {
        ++k;
        temp >>= 1;
    }
    /* temp now holds the odd part of n */
    seq_t prod_odd = odd_factor_product((seq_t)temp);  /* product of (4e_i+1) for odd primes */

    seq_t ans;
    if (k == 0) {
        /* n is odd */
        ans = (prod_odd - 1) / 2;
    } else {
        /* n is even */
        seq_t factor = (seq_t)(4 * k - 1);
        ans = (factor * prod_odd - 1) / 2;
    }
    return ans;
}
