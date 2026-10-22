/*
 * A358216  Inverse Möbius transform of A327936, where A327936 is
 *          multiplicative with a(p^e) = p if e >= p, otherwise 1.
 * https://oeis.org/A358216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∑_{d|n} A327936(d); Multiplicative with a(p^e) = e+1 if
 *            e<p, = p*(e-p+2) otherwise
 * Range:     u8 n <= 9993, u16 n <= 9993, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for out‑of‑range indices */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    if (m == 1) return 1;         /* a(1) = 1 */

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++e; }
        if (e < 2)                /* e < p = 2  */
            result *= (seq_t)(e + 1);
        else
            result *= (seq_t)2 * (seq_t)e;   /* p*(e-p+2) = 2*(e-2+2) */
    }

    /* trial division by odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t factor;
            if (e < p)
                factor = (seq_t)(e + 1);
            else
                factor = (seq_t)p * (seq_t)(e - p + 2);
            result *= factor;
        }
    }

    /* remaining prime factor with exponent 1 */
    if (m > 1) {
        /* e = 1, p = m > 1 ⇒ e < p ⇒ factor = e+1 = 2 */
        result *= (seq_t)2;
    }

    return result;
}
