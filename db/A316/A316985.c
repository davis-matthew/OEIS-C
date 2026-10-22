/*
 * A316985  Number of solutions to x^2 + y^2 - z^2 == 1 (mod n).
 * https://oeis.org/A316985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative. For prime power * a(2^1)=4, a(2^2)=24, *
 *            a(2^e)=2^(2e+1) for e>2. * For odd prime p,
 *            a(p^e)=(p+1)*p^(2e-1). * For general n, a(n)=∏ a(p^e) over the
 *            prime power factorisation of n
 * Range:     u8 n <= 11, u16 n <= 167, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;                /* a(1)=1 */

    /* factorise n into primes p_i^e_i */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;

        if (p == 2) {
            if (e == 1)
                term = 4;
            else if (e == 2)
                term = 24;
            else        /* e > 2 */
                term = pow2((int)(2 * e + 1));   /* 2^(2e+1) */
        } else {                    /* odd prime */
            int exp = (int)(2 * e - 1);          /* p^(2e-1) */
            term = (seq_t)(p + 1) * pow_seq((seq_t)p, exp);
        }

        result *= term;
    }

    return result;
}
