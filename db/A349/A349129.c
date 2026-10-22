/*
 * A349129  a(n) = Sum_{d|n} A003958(d) * A003959(n/d), where A003958 is fully
 *          multiplicative with a(p) = (p-1), and A003959 is fully
 *          multiplicative with a(p) = (p+1).
 * https://oeis.org/A349129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} ((p+1)^(e+1) - (p-1)^(e+1))/2
 * Range:     u8 n <= 22, u16 n <= 959, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* multiplicative, with a(1) = 1 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but keep safe */
        return 0;

    uint64_t nn = (uint64_t)n;
    /* factors and exponents: maximum 64 primes for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor 2 */
    if ((nn & 1) == 0) {
        uint64_t e = 0;
        do {
            e++;
            nn >>= 1;
        } while ((nn & 1) == 0);
        primes[cnt] = 2;
        exps[cnt]   = e;
        cnt++;
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= nn; d += 2) {
        if (nn % d == 0) {
            uint64_t e = 0;
            do {
                e++;
                nn /= d;
            } while (nn % d == 0);
            primes[cnt] = d;
            exps[cnt]   = e;
            cnt++;
        }
    }
    /* remaining prime factor */
    if (nn > 1) {
        primes[cnt] = nn;
        exps[cnt]   = 1;
        cnt++;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        seq_t e  = (seq_t)exps[i];
        seq_t pow_plus  = (seq_t)1;
        seq_t pow_minus = (seq_t)1;
        seq_t base_plus  = p + (seq_t)1;
        seq_t base_minus = p - (seq_t)1;
        /* exponent e+1 */
        seq_t k = (seq_t)0;
        for (k = (seq_t)0; k < e + (seq_t)1; ++k) {
            pow_plus  *= base_plus;
            pow_minus *= base_minus;
        }
        seq_t term = (pow_plus - pow_minus) / (seq_t)2;
        result *= term;
    }

    return result;
}
