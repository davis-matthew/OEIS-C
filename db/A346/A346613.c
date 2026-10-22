/*
 * A346613  Inverse Moebius transform of A019554.
 * https://oeis.org/A346613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 for n = 1; for n > 1, let n = ∏ p_i^{e_i}. Then a(n) =
 *            ∏ a(p_i^{e_i}) where a(p^e) = 2*(p^((e+1)/2+1)-1)/(p-1) -
 *            p^((e+1)/2) - 1 if e is odd, a(p^e) = 2*(p^(e/2+1)-1)/(p-1) - 1
 *            if e is even
 * Range:     u8 n <= 12, u16 n <= 4855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Arrays for factoring n. 64-bit numbers have at most 20 distinct prime factors,
       128 slots is more than enough. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t     cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        idx_t e  = exps[i];

        seq_t k;
        if (e & 1)          /* e is odd */
            k = (seq_t)((e + 1) / 2);
        else
            k = (seq_t)(e / 2);

        seq_t kp1 = k + (seq_t)1;              /* k + 1 */
        seq_t p_kp1 = big_pow(p, kp1);         /* p^(k+1)                     */
        seq_t term = (p_kp1 - (seq_t)1) * (seq_t)2;  /* 2*(p^(k+1)-1)            */
        seq_t denom = p - (seq_t)1;                 /* p-1                       */
        term /= denom;                            /* integer division          */

        if (e & 1) { /* odd */
            seq_t p_k = big_pow(p, k); /* p^k */
            term -= p_k;
            term -= (seq_t)1;
        } else { /* even */
            term -= (seq_t)1;
        }
        result *= term;
    }

    return result;
}
