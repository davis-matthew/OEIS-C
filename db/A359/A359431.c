/*
 * A359431  a(n) = A325973(n) - A326043(n).
 * https://oeis.org/A359431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A325973(n) - A326043(n), where A325973(n) =
 *            (A034448(n)+A048250(n))/2 with A034448 = prod(1+p^e), A048250 =
 *            prod(1+p), and A326043 is multiplicative with a(p^e) =
 *            floor((e-1+sigma(p^e))/e)
 * Range:     u8 n <= 767, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9286 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t U = 1, S = 1, C = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            seq_t sg = 1, pp = 1;
            for (idx_t k = 1; k <= e; ++k) {
                pp *= (seq_t)p;
                sg += pp;
            }
            U *= (seq_t)1 + pp;
            S *= (seq_t)1 + (seq_t)p;
            C *= ((seq_t)(e - 1) + sg) / (seq_t)e;
        }
    }
    if (m > 1) {
        seq_t t = (seq_t)1 + (seq_t)m;
        U *= t;
        S *= t;
        C *= t;
    }
    seq_t mean = U / 2 + S / 2 + (seq_t)(U & 1);
    return mean - C;
}
