/*
 * A395768  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-4,.) mod 4.
 * https://oeis.org/A395768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) where f(2,e) = 2^e, f(p,e) =
 *            p^(e-1)*(p-1) if p ≡ 1 (mod 4), p^(e-1)*(p+1) if p ≡ 3 (mod 4)
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not used for n ≥ 1 */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* factor out powers of 2 */
    while ((m & 1ULL) == 0) {
        result *= 2;
        m >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute p^(e-1) */
            seq_t pow_pe1 = 1;
            for (uint64_t i = 0; i + 1 < e; ++i) {
                pow_pe1 *= (seq_t)p;
            }
            seq_t part = pow_pe1;
            if (p % 4 == 1)
                part *= (seq_t)(p - 1);
            else
                part *= (seq_t)(p + 1);
            result *= part;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        seq_t part = (seq_t)(m % 4 == 1 ? (m - 1) : (m + 1));
        result *= part;
    }

    return result;
}
