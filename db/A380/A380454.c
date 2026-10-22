/*
 * A380454  a(n) = 1 if the product of exponents in its prime factorization is
 *          greater than 3, otherwise 0.
 * https://oeis.org/A380454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [product of exponents in prime factorization > 3]
 * Range:     u8 n <= 9963, u16 n <= 9963, u32 n <= 9963, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Buffers for at most 64 distinct primes of a 64‑bit integer */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t fac_count;
    uint64_t nn = (uint64_t)n;
    seq_t prod = (seq_t)1;

    /* n = 1 has no prime factors; product = 1 ≤ 3 → term is 0 */
    if (nn <= 1)
        return (seq_t)0;

    /* factor the number */
    factor_u64(nn, primes, exps, &fac_count);

    /* compute the product of exponents, stop early if it exceeds 3 */
    for (size_t i = 0; i < fac_count; ++i) {
        prod *= (seq_t)exps[i];
        if (prod > (seq_t)3)
            break;
    }

    return (prod > (seq_t)3) ? (seq_t)1 : (seq_t)0;
}
