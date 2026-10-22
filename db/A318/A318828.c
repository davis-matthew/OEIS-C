/*
 * A318828  a(n) = n - A063994(n) = n - Product_{primes p dividing n} gcd(p-1,
 *          n-1).
 * https://oeis.org/A318828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A063994(n) = n - ∏_{p|n} gcd(p-1, n-1)
 * Range:     u8 n <= 257, u16 n <= 9796, u32 n <= 9796, u64 n <= 9796,
 *            u128 n <= 9796, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, but safe */

    /* arrays large enough for any n <= UINT64_MAX (max 64 distinct primes) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n, filling primes[] and exps[]; cnt holds the number of distinct primes */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t product = (seq_t)1;   /* product over empty set is 1 */
    seq_t n_minus_1 = (seq_t)(n - 1);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_minus_1 = (seq_t)(primes[i] - 1);
        seq_t g = gcd(p_minus_1, n_minus_1);
        product = product * g;
    }

    return (seq_t)n - product;
}
