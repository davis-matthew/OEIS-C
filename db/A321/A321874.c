/*
 * A321874  If n = Product (p_j^k_j) then a(n) = Product
 *          (prime(p_j)^prime(k_j)).
 * https://oeis.org/A321874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} prime(p)^prime(e)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined, return 0. */
    if (n < 1)
        return 0;

    /* Empty product gives 1 for n == 1. */
    if (n == 1)
        return 1;

    /* Factor the integer n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t base = (seq_t)get_prime(primes[i]);   /* prime(p)   */
        seq_t exp  = (seq_t)get_prime(exps[i]);     /* prime(e)   */
        seq_t term = big_pow(base, exp);            /* base^exp   */
        result *= term;
    }

    return result;
}
