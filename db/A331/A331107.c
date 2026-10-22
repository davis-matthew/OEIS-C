/*
 * A331107  The sum of Zeckendorf-infinitary divisors of n = Product_{i}
 *          p(i)^r(i): divisors d = Product_{i} p(i)^s(i), such that the
 *          Zeckendorf expansion (A014417) of each s(i) contains only terms
 *          that are in the Zeckendorf expansion of r(i).
 * https://oeis.org/A331107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_p a(p^e), with a(p^e) = ∏_{s∈Z(e)} (p^s + 1), where
 *            Z(e) is the Zeckendorf representation of the exponent e
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Fibonacci numbers used for Zeckendorf decompositions.
   F(1)=1, F(2)=2, so the array starts with 1,2,3,5,8,... */
static const idx_t fib[20] = {
    1, 2, 3, 5, 8, 13, 21, 34, 55, 89,
    144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946
};

seq_t a(idx_t n)
{
    /* Handle the trivial case.  For n=1 the loop below will
       produce the correct result anyway; the explicit check
       keeps the function short for small n. */
    if (n == 1) return 1;

    /* Factorise n.  The helper accepts a 64‑bit unsigned input
       and returns arrays of primes and corresponding exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p     = primes[i];
        idx_t    e     = (idx_t)exps[i];
        seq_t    contrib = 1;
        idx_t    rem     = e;

        /* Zeckendorf decomposition: subtract the largest Fibonacci
           number that does not exceed the remainder. */
        for (int j = (int)sizeof(fib) / (int)sizeof(fib[0]) - 1; j >= 0; --j) {
            if (rem >= fib[j]) {
                seq_t powp = pow_uint(p, fib[j]); /* p^fib[j] */
                powp += 1;                       /* p^s + 1 */
                contrib *= powp;
                rem -= fib[j];
            }
        }
        result *= contrib;
    }

    return result;
}
