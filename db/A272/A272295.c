/*
 * A272295  a(n) = v(2*n+1), where v is a completely multiplicative sequence
 *          defined by v(2)=0 and for prime p >= 3: v(p^e) = ((p+1)/2)^e when
 *          p == 1 mod 4, and v(p^e) = ((1-p)/2)^e when p == 3 mod 4.
 * https://oeis.org/A272295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = v(2*n+1) where * v is completely multiplicative with *
 *            v(2)=0 * and for odd prime p ≥ 3 * v(p^e) = ((p+1)/2)^e if p ≡ 1
 *            (mod 4) * v(p^e) = ((1-p)/2)^e if p ≡ 3 (mod 4) * * Since 2*n+1
 *            is odd, v(2) never contributes. * This implementation factors
 *            2*n+1 by trial division * and multiplies the...
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2*n+1 fits in 64‑bit for the test range. */
    uint64_t m = 2 * n + 1;
    if (m == 1)
        return 1;                     /* v(1) = 1 */

    seq_t result = 1;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp == 0)
            continue;
        seq_t base;
        if ((p % 4) == 1) {
            base = ((seq_t)p + 1) / 2;            /* (p+1)/2 */
        } else {                                 /* p ≡ 3 (mod 4) */
            base = ((seq_t)1 - (seq_t)p) / 2;     /* (1-p)/2 */
        }
        seq_t pow = 1;
        for (int i = 0; i < exp; ++i)
            pow *= base;
        result *= pow;
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        seq_t base;
        if ((m % 4) == 1) {
            base = ((seq_t)m + 1) / 2;
        } else {
            base = ((seq_t)1 - (seq_t)m) / 2;
        }
        result *= base;
    }

    return result;
}
