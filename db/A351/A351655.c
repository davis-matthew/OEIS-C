/*
 * A351655  Dirichlet g.f.: Product_{p prime} 1 / (1 - p^(-s) - p^(-2*s) -
 *          p^(-3*s)).
 * https://oeis.org/A351655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e | n of tribonacci(e+2) *
 *            where tribonacci(k) = T(k) with T(0)=0, T(1)=0, T(2)=1 * (OEIS
 *            A000073)
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, tribonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/tribonacci.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* graceful handling of n==0 if called */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        /* e = exponent of prime p^e dividing n */
        uint64_t e = exps[i];
        /* compute tribonacci(e+2) */
        seq_t t = tribonacci((uint32_t)(e + 2));
        res = res * t;
    }
    return res;
}
