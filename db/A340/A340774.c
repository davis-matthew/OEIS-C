/*
 * A340774  Dirichlet g.f.: Sum_{n>0} a(n)/n^s = zeta(s-1) * zeta(2*s-1).
 * https://oeis.org/A340774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * \sum_{d^2|n} 1/d * Equivalent multiplicative: a(p^e)
 *            = (p^(e+1)-p^floor((e+1)/2))/(p-1)
 * Range:     u8 n <= 16, u16 n <= 3731, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset, but safe */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t p_seq = (seq_t)p;
            uint64_t f = (e + 1) / 2; /* floor((e+1)/2) */
            seq_t part = (pow_int(p_seq, (idx_t)(e + 1)) - pow_int(p_seq, (idx_t)f))
                         / (p_seq - (seq_t)1);
            result *= part;
        }
    }

    if (m > 1) {  /* remaining prime factor with exponent 1 */
        seq_t p_seq = (seq_t)m;
        seq_t part = (pow_int(p_seq, 2) - p_seq) / (p_seq - (seq_t)1);
        result *= part;
    }

    return result;
}
