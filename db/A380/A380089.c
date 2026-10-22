/*
 * A380089  The number of unitary divisors of n that are terms in A207481.
 * https://oeis.org/A380089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A380089:  a(n) = number of unitary divisors of n that belong to A207481.
 *  Multiplicative definition:  a(p^e) = 2 if e ≤ p, otherwise 1.
 *  For n > 0, a is computed from the prime factorisation of n.  */

/* Using the library routine that factors u64 integers.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* declares factor_u64 */

seq_t a(idx_t n)
{
    /*  OEIS offset 1: the sequence is undefined for n = 0.
     *  Returning 0 for n = 0 keeps the function well‑defined.  */
    if (n == 0u)
        return 0;

    /*  Factor the argument.  The helper stores distinct primes
     *  and their exponents in the supplied arrays.  */
    uint64_t u = (uint64_t)n;
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(u, primes, exps, &cnt);

    /*  Apply the multiplicative rule.  For each prime power p^e,
     *  multiply the result by 2 iff e ≤ p, otherwise leave it unchanged.  */
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] <= primes[i])
            res *= (seq_t)2;
    }
    return res;
}
