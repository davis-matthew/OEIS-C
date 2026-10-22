/*
 * A390866  The number of integers k from 1 to n such that gcd(n, k) is a
 *          nonexponential divisor of n.
 * https://oeis.org/A390866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - ∏_{p^e‖n} ( ∑_{d|e, d>0} φ(p^{e−d}) )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ(p^k) = (p−1)p^{k−1} for k>0 and φ(1)=1 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t    cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* product over all primes */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* sum over positive divisors of e */
        seq_t sum = 0;
        for (uint64_t f = 1; f <= e; ++f) {
            if (e % f == 0) {
                uint64_t exp = e - f;          /* exponent of p in n/d   */
                seq_t phi;
                if (exp == 0) {
                    phi = 1;                   /* φ(1) = 1              */
                } else {
                    seq_t power = seq_pow((seq_t)p, (idx_t)(exp - 1));
                    phi = (seq_t)(p - 1) * power;
                }
                sum += phi;
            }
        }
        prod *= sum;
    }

    /* result */
    seq_t result = (seq_t)n - prod;
    return result;
}
