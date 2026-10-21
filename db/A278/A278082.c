/*
 * A278082  1/12 of the number of primitive quadruples with sum = n and sum of
 *          squares = 3*n^2.
 * https://oeis.org/A278082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is multiplicative. a(2) = 1, a(2^k) = 0 for k >= 2; a(p^k)
 *            = p^(k-1) * a(p). For odd prime p: a(p) = p + 1 if p ≡
 *            2,6,7,8,10 (mod 11); a(p) = p - 1 if p ≡ 1,3,4,5,9 (mod 11);
 *            a(11) = 11
 * Range:     u8 n <= 246, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0, but guard */
    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    uint64_t e2 = 0;
    while ((m & 1ULL) == 0ULL) {          /* m % 2 == 0 */
        e2++;
        m >>= 1;                          /* m /= 2 */
    }
    if (e2 >= 2) return 0;                /* zero if 4 divides n */

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0) continue;
        uint64_t e = 0;
        while (m % p == 0) {
            e++;
            m /= p;
        }

        seq_t base;
        if (p == 11ULL)
            base = 11;
        else {
            uint64_t r = p % 11ULL;
            if (r == 1ULL || r == 3ULL || r == 4ULL || r == 5ULL || r == 9ULL)
                base = (seq_t)(p - 1ULL);
            else
                base = (seq_t)(p + 1ULL);
        }

        seq_t factor = base;
        for (uint64_t i = 1; i < e; ++i)
            factor *= (seq_t)p;

        result *= factor;
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;
        seq_t base;
        if (p == 11ULL)
            base = 11;
        else {
            uint64_t r = p % 11ULL;
            if (r == 1ULL || r == 3ULL || r == 4ULL || r == 5ULL || r == 9ULL)
                base = (seq_t)(p - 1ULL);
            else
                base = (seq_t)(p + 1ULL);
        }
        result *= base;
    }

    return result;
}
