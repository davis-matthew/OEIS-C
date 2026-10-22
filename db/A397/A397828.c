/*
 * A397828  Reduced Dedekind psi function: a(p1^e1*...*pN^eN) =
 *          lcm((p1+1)*p1^(e1-1), ..., (pN+1)*pN^(eN-1)).
 * https://oeis.org/A397828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm{ (p+1)*p^(e-1) : p^e || n }
 * Range:     u8 n <= 193, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    /* factor n: we allocate arrays large enough for any 64‑bit number
       (no more than 64 distinct prime factors). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p_pow = seq_pow(p, (idx_t)(exps[i] - 1));   /* p^(e-1), 0 if e==1 */
        seq_t part = (p + (seq_t)1) * p_pow;            /* (p+1)*p^(e-1) */

        seq_t g = gcd(result, part);
        result = (result / g) * part;                  /* lcm(result,part) */
    }

    return result;
}
