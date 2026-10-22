/*
 * A329272  Number of octic primitive Dirichlet characters modulo n.
 * https://oeis.org/A329272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e * f(2^e) = 2^(e-2)
 *            for 2 ≤ e ≤ 5, * 0 otherwise, * f(p^e) = gcd(p-1,8)-1 for e=1 (p
 *            odd prime), * 0 for e≥2. The value a(1)=1
 * Range:     u8 n <= 9344, u16 n <= 9344, u32 n <= 9344, u64 n <= 9344,
 *            u128 n <= 9344, big any n
 * Helpers:   _gcd_int_small, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/_gcd_int_small.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* factor out powers of two */
    idx_t m = n;
    int e2 = 0;
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }

    /* zero for 2^1 or 2^e with e≥6 */
    if (e2 == 1 || e2 >= 6) return 0;

    seq_t result = 1;
    if (e2 >= 2)                     /* e2 in [2,5] */
        result *= (seq_t)(1ULL << (e2 - 2));

    /* factor odd part */
    while (m > 1) {
        /* find the smallest odd prime factor of m */
        idx_t p = 0;
        for (idx_t cand = 3; cand * cand <= m; cand += 2) {
            if (m % cand == 0) {
                p = cand;
                break;
            }
        }
        if (p == 0) p = m;          /* m itself is prime */

        int exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        if (exp > 1) return 0;       /* a prime power with e≥2 gives zero */

        /* compute gcd(p-1,8)-1 */
        int mod8 = (int)((p - 1) & 7ULL);
        int g = _gcd_int_small(mod8, 8);
        int contrib = g - 1;
        result *= (seq_t)contrib;
    }

    return result;
}
