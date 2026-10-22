/*
 * A396079  Product of the least factors in all proper factorizations of n
 *          into factors > 1, up to permutation; a(n) = 1 for noncomposite n.
 * https://oeis.org/A396079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{f|n, 2≤f<n} f^{cnt(n/f, f)}, where cnt(m, f) is the
 *            number of unordered factorizations of m into factors ≥ f
 *            (including the trivial 1‑term factorization [m])
 * Range:     u8 n <= 35, u16 n <= 71, u32 n <= 143, u64 n <= 287,
 *            u128 n <= 719, big any n
 * Helpers:   _count_factorizations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_count_factorizations.h"

/* Fast exponentiation for seq_t. */
static seq_t _pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n==1 (or 0) returns 1 by definition. */
    if (n < 2)
        return 1;

    seq_t product = 1;
    uint64_t n64 = (uint64_t)n;

    /* Enumerate all divisors f of n such that 2 ≤ f < n. */
    for (uint64_t f = 2; f * f <= n64; ++f) {
        if (n64 % f == 0) {
            uint64_t m1 = n64 / f;
            /* Factorization with minimal factor f. */
            uint64_t exp1 = _count_factorizations(m1, f);
            if (exp1 > 0) {
                seq_t pow1 = _pow_seq_t((seq_t)f, exp1);
                product *= pow1;
            }

            /* Complementary divisor m1 (> sqrt). Handle it as f2 = m1. */
            if (f != m1 && m1 > 1 && m1 < n64) {
                uint64_t f2 = m1;
                uint64_t m2 = n64 / f2; /* which equals f */
                uint64_t exp2 = _count_factorizations(m2, f2);
                if (exp2 > 0) {
                    seq_t pow2 = _pow_seq_t((seq_t)f2, exp2);
                    product *= pow2;
                }
            }
        }
    }

    /* Nothing else to do; if n is prime, product stays 1. */
    return product;
}
