/*
 * A319626  Primorial deflation of n (numerator): Let f be the completely
 *          multiplicative function over the positive rational numbers defined
 *          by f(p) = A034386(p) for any prime number p; f constitutes a
 *          permutation of the positive rational numbers; let g be the inverse
 *          of f; for any n > 0, a(n) is the numerat...
 * https://oeis.org/A319626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, A064989(n)) where A064989(n) = ∏_{p^e‖n, p>2}
 *            prev(p)^e The implementation uses the prime factorisation of n.
 *            For each prime r dividing n the denominator contributes prev(r)
 *            to the product; equivalently, the GCD contains r to the power
 *            min(exp_n(r), exp_n(next_prime(r))). Hence...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* fast exponentiation for seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1ULL)
            res *= base;
        exp >>= 1ULL;
        if (exp)
            base *= base;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined offset */
    if (n == (idx_t)1)
        return (seq_t)1;          /* a(1) = 1 */

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t pr = primes[i];
        idx_t exp_n   = (idx_t)exps[i];

        /* exponent of next prime in n */
        uint64_t next_pr = next_prime(pr);
        idx_t exp_next = 0;
        for (size_t j = 0; j < cnt; ++j)
        {
            if (primes[j] == next_pr)
            {
                exp_next = (idx_t)exps[j];
                break;
            }
        }

        idx_t g = exp_n;
        if (exp_next < g)
            g = exp_next;           /* min(exp_n, exp_next) */

        idx_t exp_res = exp_n - g; /* exponent to keep in the result */

        result *= pow_seq(pr, exp_res);
    }

    return result;
}
