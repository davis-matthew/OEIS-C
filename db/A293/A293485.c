/*
 * A293485  The number of 8th powers in the multiplicative group modulo n.
 * https://oeis.org/A293485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) computed multiplicatively: * a(2^e) = 1 (e <= 4) , * a(2^e)
 *            = 2^(e-5) (e >= 5) * For odd prime p: * a(p^e) = (p-1)*p^(e-1)/d
 *            * where d = 8 if p≡1 (mod8), * d = 4 if p≡5 (mod8), * d = 2 if
 *            p≡3 or 7 (mod8) * The sequence is a product over all prime
 *            powers dividing n
 * Range:     u8 n <= 522, u16 n <= 9984, u32 n <= 9984, u64 n <= 9984,
 *            u128 n <= 9984, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined, but never called with n<1 in OEIS range */

    uint64_t nn = (uint64_t)n;

    /* factor n into primes: primes[i]^exps[i], i=0..cnt-1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
#if defined(__cplusplus)
    factor_u64(nn, primes, exps, &cnt);
#else
    /* factor_u64 expects a pointer to size_t, so pass the address. */
    factor_u64(nn, primes, exps, &cnt);
#endif

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t contrib = 1;

        if (p == 2ULL) {
            if (e <= 4ULL) {
                contrib = 1;
            } else {
                /* 2^(e-5) */
                contrib = (seq_t)1 << (unsigned)(e - 5ULL);
            }
        } else {
            /* compute φ(p^e) = (p-1)*p^(e-1) */
            seq_t power = 1;           /* p^(e-1) */
            seq_t pp = (seq_t)p;
            for (uint64_t k = 1; k < e; ++k)
                power *= pp;
            seq_t phi = ( (seq_t)(p - 1) ) * power;

            /* determine divisor d based on p mod 8 */
            unsigned mod8 = (unsigned)(p & 7ULL);
            unsigned d;
            if (mod8 == 1U)
                d = 8U;
            else if (mod8 == 5U)
                d = 4U;
            else
                d = 2U;   /* p ≡ 3 or 7 (mod 8) */

            contrib = phi / (seq_t)d;
        }

        result *= contrib;
    }

    return result;
}
