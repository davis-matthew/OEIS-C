/*
 * A353379  Primepi-based variant of the arithmetic derivative applied to the
 *          prime shadow of n.
 * https://oeis.org/A353379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258851(A181819(n)); with m = A181819(n) = Product
 *            prime(e_i) for n = Product p_i^e_i, a(n) = Sum_i e_i * m /
 *            prime(e_i), since pi(prime(e)) = e
 * Range:     u8 n <= 10079, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 115 terms through n = 9851 (2026-11-10)
 * Helpers:   factor_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t m = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        m = m * (seq_t)nth_prime((idx_t)ex[i]);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t q = (seq_t)nth_prime((idx_t)ex[i]);
        result += (seq_t)ex[i] * (m / q);
    }
    return result;
}
