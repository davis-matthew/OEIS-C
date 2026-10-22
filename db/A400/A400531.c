/*
 * A400531  a(n) = chi_{n,-1}(-1), where chi_{n,-1} is the Dirichlet character
 *          modulo n with label -1 in the Conrey labeling system.
 * https://oeis.org/A400531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult, new
 * Formula:   a(n) = χ_{n,-1}(-1) Multiplicative: a(p^e)= 1 if p≡1 (mod 4) or
 *            p=2 and e=1, -1 if p≡3 (mod 4) or p=2 and e≥2. For a general n,
 *            factor it into prime powers and multiply the corresponding signs
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* n is at least 1.  Handle n=1 directly. */
    if (n <= 1ULL)
        return (seq_t)1;

    /* Arrays large enough for all distinct primes of a 64‑bit number. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    /* Factor n. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2ULL) {
            /* exponent is in exps[i] */
            result *= (exps[i] == 1ULL) ? (seq_t)1 : (seq_t)-1;
        } else {
            /* For odd primes, exponent does not affect sign. */
            result *= (p % 4ULL == 1ULL) ? (seq_t)1 : (seq_t)-1;
        }
    }

    return result;
}
