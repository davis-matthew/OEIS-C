/*
 * A376074  a(n) is the number of distinct right circular cones with integer
 *          radius and height having the same volume as a sphere with radius
 *          n.
 * https://oeis.org/A376074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions (x,y)∈ℕ² of x²⋅y = 4·n³. If n = ∏pᵉ,
 *            for p = 2 : factor 2 + 3·e; for p ≠ 2 : factor 3·e. Each prime
 *            contributes floor(factor/2)+1 to the product
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, but safe */

    /* Separate the exponent of 2 */
    idx_t rem = n;
    int exp2 = 0;
    while ((rem & 1ULL) == 0) {   /* rem % 2 == 0 */
        exp2++;
        rem >>= 1;                /* divide by 2 */
    }

    /* Start with the contribution of prime 2 */
    seq_t result = 1;
    seq_t factor2 = ((seq_t)( (2ULL + 3ULL * (uint64_t)exp2) / 2ULL ) + 1ULL);
    result *= factor2;

    /* If the remaining part is 1, we are done */
    if (rem == 1) return result;

    /* Generate primes up to √rem and factor the odd part */
    idx_t limit = _floor_isqrt_u64(rem);
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > rem) break;
        int exp = 0;
        while (rem % p == 0) {
            exp++;
            rem /= p;
        }
        if (exp > 0) {
            uint64_t eprime = 3ULL * (uint64_t)exp;
            seq_t factor = ((seq_t)(eprime / 2ULL) + 1ULL);
            result *= factor;
        }
    }

    /* If a prime factor larger than √original_n remains, its exponent is 1 */
    if (rem > 1) {
        /* For any remaining prime p, eprime = 3, so factor = floor(3/2)+1 = 2 */
        result *= (seq_t)2;
    }

    free(primes);
    return result;
}
