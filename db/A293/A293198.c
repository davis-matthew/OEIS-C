/*
 * A293198  a(n) is the least positive k such that f(k) = f(k + n) where f(k)
 *          = A000120(k) / A070939(k).
 * https://oeis.org/A293198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = least k ≥ 1 with popcount(k)/bitlen(k) =
 *            popcount(k+n)/bitlen(k+n) * where popcount counts 1‑bits in the
 *            binary expansion of k, * and bitlen(k) = ⌊log₂(k)⌋ + 1. * a(0)
 *            is defined to be 1
 * Range:     u8 n <= 79, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned popcnt_u64(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

static inline unsigned bitlen_u64(idx_t x)
{
    return x == 0 ? 0 : 64 - __builtin_clzll((unsigned long long)x);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* The assertion a(n) ≤ 5*n holds for all n ≥ 1. */
    idx_t limit = 5 * n;
    for (idx_t k = 1; k <= limit; ++k)
    {
        unsigned p1 = popcnt_u64(k);
        unsigned l1 = bitlen_u64(k);

        idx_t kn = k + n;
        unsigned p2 = popcnt_u64(kn);
        unsigned l2 = bitlen_u64(kn);

        /* Equality of the reduced fractions */
        if ((unsigned long long)p1 * l2 == (unsigned long long)p2 * l1)
            return (seq_t)k;
    }

    /* In theory this point is unreachable because the bound guarantees a solution. */
    return (seq_t)0;
}
