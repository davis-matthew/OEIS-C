/*
 * A304409  If n = Product (p_j^k_j) then a(n) = Product (p_j*(k_j + 1)).
 * https://oeis.org/A304409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^k||n} p * (k+1)
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n = 1 the product over an empty set is 1 */
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    size_t cnt = 0;
    /* These arrays are large enough for numbers up to 2^64 */
    uint64_t primes[64];
    uint64_t exps[64];

    /* factor_u64 fills primes[], exps[] and sets cnt */
    factor_u64(m, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)primes[i] * (seq_t)(exps[i] + 1);
        result = result * term;
    }
    return result;
}
