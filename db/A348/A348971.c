/*
 * A348971  a(n) = Product(p*(p+1)^(e-1)) - Product((p-1)*p^(e-1)), when n =
 *          Product(p^e), with p primes, and e their exponents.
 * https://oeis.org/A348971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Product(p*(p+1)^(e-1)) - Product((p-1)*p^(e-1)) where n =
 *            ∏ p^e over distinct primes p. This matches A003968(n) -
 *            A000010(n)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9413, u64 n <= 9413,
 *            u128 n <= 9413, big any n
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
    if (n == 0) return 0;           /* out of range but safe */
    /* arrays to hold prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t prod1 = 1;
    seq_t prod2 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        idx_t  e  = (idx_t)exps[i];
        seq_t t1, t2;

        if (e == 1) {
            t1 = p;                         /* p * (p+1)^0 */
            t2 = p - 1;                     /* (p-1) * p^0 */
        } else {
            seq_t power1 = big_pow(p + 1, (seq_t)(e - 1));
            seq_t power2 = big_pow(p,     (seq_t)(e - 1));
            t1 = p * power1;
            t2 = (p - 1) * power2;
        }
        prod1 *= t1;
        prod2 *= t2;
    }
    return prod1 - prod2;
}
