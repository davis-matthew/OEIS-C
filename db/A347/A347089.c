/*
 * A347089  a(n) = gcd(A055155(n), d(n)), where A055155(n) = Sum_{d|n} gcd(d,
 *          n/d) and d(n) gives the number of divisors of n.
 * https://oeis.org/A347089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A055155(n), d(n)) * where A055155(n) = Σ_{d|n} gcd(d,
 *            n/d) and d(n) is the number of divisors of n
 * Range:     u8 n <= 9421, u16 n <= 9421, u32 n <= 9421, u64 n <= 9421,
 *            u128 n <= 9421, big any n
 * Helpers:   _gcd_u64, _sum_divisor_gcd, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"         /* seq_t gcd(seq_t a, seq_t b)        */
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n,       */
#include "../utils/_gcd_u64.h"
#include "../utils/_sum_divisor_gcd.h"
                                 /*               uint64_t *pr,      */
                                 /*               uint64_t *ex,     */
                                 /*               size_t *cnt);     */

/* Small helper: gcd for uint64_t values. */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n = 0, guard anyway */

    /* Factorise n */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute d(n) = product (exp + 1) */
    uint64_t divcnt = 1;
    for (size_t i = 0; i < cnt; ++i)
        divcnt *= (exps[i] + 1);

    /* Compute A055155(n) = Σ_{d|n} gcd(d, n/d) */
    uint64_t sum = 0;
    _sum_divisor_gcd(0, (idx_t)1, n, primes, exps, cnt, &sum);

    /* a(n) = gcd(sum, divcnt) */
    uint64_t g = _gcd_u64(sum, divcnt);
    return (seq_t)g;
}
