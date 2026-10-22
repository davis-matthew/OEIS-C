/*
 * A370598  Characteristic function of exponentially squarefree numbers
 *          (A209061).
 * https://oeis.org/A370598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} mu(e)^2. For every exponent e of a
 *            prime factor, a(p^e) = 1 iff e is squarefree, otherwise 0. Thus
 *            a(n) = 1 iff all exponents in the prime factorisation of n are
 *            squarefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* helper: test whether integer e is squarefree (no prime square divides e) */
static bool is_exp_squarefree(uint64_t e)
{
    /* 0 and 1 are squarefree by definition */
    if (e <= 1) return true;
    for (uint64_t p = 2; p * p <= e; ++p) {
        if (e % p == 0) {
            if (e % (p * p) == 0)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if ((idx_t)n == 0)
        return (seq_t)0;          /* out of specified offset, return 0 */
    if ((idx_t)n == 1)
        return (seq_t)1;          /* a(1) = 1 by definition */

    /* factorise n, store primes and exponents */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (!is_exp_squarefree(e))
            return (seq_t)0;      /* exponent not squarefree -> sequence value 0 */
    }

    return (seq_t)1;              /* all exponents were squarefree -> value 1 */
}
