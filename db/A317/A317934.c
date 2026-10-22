/*
 * A317934  Multiplicative with a(p^n) = 2^A011371(n); denominators for
 *          certain "Dirichlet Square Roots" sequences.
 * https://oeis.org/A317934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = 2^( sum_{p^e∥n} (e - popcount(e)) )
 * Range:     u8 n <= 1023, u16 n <= 9225, u32 n <= 9225, u64 n <= 9225,
 *            u128 n <= 9225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t sum = 0;

    /* handle factor 2 */
    if ((temp & 1ULL) == 0) {
        idx_t e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        idx_t pop = 0, t = e;
        while (t) {
            pop += t & 1;
            t >>= 1;
        }
        sum += e - pop;
    }

    /* factor odd primes */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            idx_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            idx_t pop = 0, t = e;
            while (t) {
                pop += t & 1;
                t >>= 1;
            }
            sum += e - pop;
        }
    }

    /* remaining prime factor has exponent 1 → contributes 0 */

    return ((seq_t)1 << sum);
}
