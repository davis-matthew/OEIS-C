/*
 * A319676  Numerator of A047994(n)/n where A047994 is the unitary totient
 *          function.
 * https://oeis.org/A319676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of A047994(n)/n where A047994 is the unitary
 *            totient function. For n = ∏ p_i^{e_i}, let P = ∏ (p_i^{e_i} -
 *            1). Then a(n) = P / gcd(P, n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;           /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t prod = 1;

    /* factorization by trial division (fast enough for n <= ~10000) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t pk = 1;
            /* compute p^e */
            do {
                pk *= p;
                m /= p;
            } while (m % p == 0);

            seq_t term = (seq_t)pk - 1;   /* p^e - 1 */
            prod *= term;
        }
    }

    /* remaining prime factor */
    if (m > 1) {
        seq_t term = (seq_t)m - 1;
        prod *= term;
    }

    /* cancel common factors with original n */
    seq_t g = gcd(prod, (seq_t)n);
    return prod / g;
}
