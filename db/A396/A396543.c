/*
 * A396543  The number of coreful divisors of n that are not exponential
 *          divisors of n.
 * https://oeis.org/A396543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏(prime exponents of n) − ∏(τ(prime exponents of n)),
 *            where τ(k) is the number of positive divisors of k. For n = 1
 *            the value is 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Return the number of positive divisors of x (x is small, ≤ 63). */
static inline seq_t tau_uint64(uint64_t x)
{
    seq_t cnt = 0;
    for (uint64_t d = 1; d * d <= x; ++d)
    {
        if (x % d == 0)
            cnt += (d * d == x) ? 1 : 2;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* For n < 2 the definition gives 0. */
    if (n < 2)
        return 0;

    /* Factor n; the helper fills arrays with primes and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod_coreful = 1;   /* ∏ e_i   */
    seq_t prod_expdiv  = 1;   /* ∏ τ(e_i) */

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t ei = (seq_t)exps[i];
        prod_coreful *= ei;
        prod_expdiv  *= tau_uint64(exps[i]);
    }

    return prod_coreful - prod_expdiv;
}
