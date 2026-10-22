/*
 * A335341  Sum of divisors of A003557(n).
 * https://oeis.org/A335341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Σ_{d|n , n/d coreful} d Equivalent: multiply over prime
 *            powers p^e | n a(p^1) = 1, a(p^e) = 1 + p + p^2 + … + p^{e-1} (e
 *            ≥ 2)
 * Range:     u8 n <= 511, u16 n <= 9569, u32 n <= 9569, u64 n <= 9569,
 *            u128 n <= 9569, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper: factor n into primes[] and exps[] (distinct primes only). */
static void
factor_uint64(uint64_t n, uint64_t primes[64], uint64_t exps[64], size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;

    uint64_t exp = 0;
    while ((n & 1ULL) == 0) {   /* handle factor 2 */
        n >>= 1;
        exp++;
    }
    if (exp > 0) {
        primes[0] = 2;
        exps[0] = exp;
        *cnt = 1;
    }

    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            primes[*cnt] = p;
            exps[*cnt] = exp;
            (*cnt)++;
        }
    }
    if (n > 1) {  /* remaining prime factor */
        primes[*cnt] = n;
        exps[*cnt] = 1;
        (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    uint64_t un = (uint64_t)n;

    /* By definition a(1)=1 */
    if (un <= 1) return (seq_t)1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64(un, primes, exps, &cnt);

    /* Compute multiplicative result */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t term = 1;       /* term for this prime */

        if (e > 1) {          /* e == 1 -> term stays 1 */
            seq_t pow = 1;    /* p^(k-1) for k=1..e-1 */
            for (uint64_t k = 1; k < e; ++k) {
                pow  = pow * (seq_t)p;
                term = term + pow;
            }
        }
        result = result * term;
    }
    return result;
}
