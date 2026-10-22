/*
 * A384042  The number of integers k from 1 to n such that gcd(n,k) is a
 *          5-rough number (A007310).
 * https://oeis.org/A384042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e||n, a(p^e) =
 *            (p-1)*p^(e-1) if p <= 3 p^e otherwise
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* no prime factors → product is 1 */

    /* arrays for prime factors and their exponents */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;

    /* factor n (works for large uint64_t values) */
    factor_u64(n, pr, ex, &cnt);

    seq_t prod = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        idx_t e = (idx_t)ex[i];

        seq_t term;
        if (pr[i] <= 3) {                /* p = 2 or 3 */
            term = (p - (seq_t)1) * seq_pow(p, e - 1);
        } else {                         /* p >= 5 */
            term = seq_pow(p, e);
        }
        prod *= term;
    }
    return prod;
}
