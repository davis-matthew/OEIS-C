/*
 * A345451  Sum of the unitary divisors of n whose square does not divide n.
 * https://oeis.org/A345451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_{p^e‖n} (1 + p^e)) - 1 i.e. sum of unitary divisors of
 *            n minus 1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n == 1, the product is 1 and the result becomes 0. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* Factor n into prime powers. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t ppe = pow_uint(primes[i], exps[i]);   /* p^e */
        prod = prod * (ppe + (seq_t)1);             /* multiply by (1+p^e) */
    }

    return prod - (seq_t)1;
}
