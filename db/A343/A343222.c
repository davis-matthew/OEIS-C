/*
 * A343222  Number of iterations of x -> A003961(x) needed until A003415(x) <=
 *          x, when starting from x=n, where A003415(x) gives the arithmetic
 *          derivative of x, and A003961 shifts its prime factorization one
 *          step towards the larger primes.
 * https://oeis.org/A343222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 0 | A003415( A003961^k(n) ) ≤ A003961^k(n)}
 * Range:     u8 n <= 31, u16 n <= 8178, u32 n <= 8178, u64 n <= 8178,
 *            u128 n <= 8178, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t arithmetic_derivative(uint64_t x)
{
    if (x <= 1)
        return 0;
    /* factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(x / primes[i]) * (seq_t)exps[i];
        der += term;
    }
    return der;
}

static seq_t shift_prime_factorization(uint64_t x)
{
    if (x <= 1)
        return 1; /* identity for 1, not used often */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t nextp = next_prime(primes[i]);

        /* compute nextp ^ exps[i] */
        seq_t pow = 1;
        for (uint64_t j = 0; j < exps[i]; ++j)
            pow *= (seq_t)nextp;

        result *= pow;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined at offset, but safe return */

    uint64_t x = (uint64_t)n;
    seq_t cnt = 0;

    while (true) {
        seq_t d = arithmetic_derivative(x);
        if (d <= x)
            break;
        x = (uint64_t)shift_prime_factorization(x);
        ++cnt;
    }
    return cnt;
}
