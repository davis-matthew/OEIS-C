/*
 * A373007  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4, x_5 <= n} ( n/gcd(x_1, x_2,
 *          x_3, x_4, x_5, n) )^2.
 * https://oeis.org/A373007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \sum_{d\,|\,n} \mu(n/d)\,(n/d)^2\,\sigma_7(d)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 566,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = \prod_{p^e\,\parallel\,n} \frac{p^{7e+7}-p^{7e+2}+p^2-1}{p^7-1} */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, unused */
    uint64_t N = (uint64_t)n;

    /* factor n into primes ± exponents */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(N, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = ex[i];

        /* compute p^7 as 128‑bit value */
        unsigned __int128 p7 = 1;
        for (int j = 0; j < 7; ++j) p7 *= (unsigned __int128)p;

        /* f(e) = a(p^e), with recurrence
           f(0) = 1, f(k) = p^7 * f(k-1) - (p^2-1)  for k>=1            */
        unsigned __int128 f = 1; /* f(0) */
        unsigned __int128 p2_minus1 = (unsigned __int128)p * p - 1;
        for (uint64_t k = 1; k <= e; ++k) {
            f = p7 * f - p2_minus1;
        }

        /* multiply into the final result, using 128‑bit to avoid overflow */
        result = (seq_t)((unsigned __int128)result * f);
    }
    return result;
}
