/*
 * A373472  a(n) = 1 if A001414(n) and A059975(n) are both multiples of 3,
 *          otherwise 0, where A001414 and A059975 are fully additive with
 *          a(p) = p and a(p) = p-1, respectively.
 * https://oeis.org/A373472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if Sum_{p^e||n} e·p ≡ 0 (mod 3) and Sum_{p^e||n}
 *            e·(p−1) ≡ 0 (mod 3), otherwise 0. The sums are taken over the
 *            prime factorization of n
 * Range:     u8 n <= 9503, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Work in unsigned 64‑bit integers for factorization; the result is
       always 0 or 1 so we cast at the end. */
    unsigned long long m = (unsigned long long)n;
    unsigned int mod1 = 0, mod2 = 0;   /* residues modulo 3 */

    /* Handle factor 2 separately. */
    if (m % 2 == 0) {
        unsigned int cnt = 0;
        while (m % 2 == 0) {
            ++cnt;
            m /= 2;
        }
        mod1 = (mod1 + cnt * (unsigned int)(2 % 3)) % 3;
        mod2 = (mod2 + cnt * (unsigned int)((2 - 1) % 3)) % 3;
    }

    /* Odd divisors. */
    for (unsigned long long d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            unsigned int cnt = 0;
            while (m % d == 0) {
                ++cnt;
                m /= d;
            }
            mod1 = (mod1 + cnt * (unsigned int)(d % 3)) % 3;
            mod2 = (mod2 + cnt * (unsigned int)(((d - 1) % 3))) % 3;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        mod1 = (mod1 + (unsigned int)(m % 3)) % 3;
        mod2 = (mod2 + (unsigned int)((m - 1) % 3)) % 3;
    }

    return (seq_t)((mod1 == 0 && mod2 == 0) ? 1 : 0);
}
