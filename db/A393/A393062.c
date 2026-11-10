/*
 * A393062  a(n) = A276085(LCM_{d|n} A276086(d)), where LCM is the least
 *          common multiple, A276086 is the primorial base exp-function, and
 *          A276085 is its left inverse.
 * https://oeis.org/A393062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k>=0} m_k * A002110(k), where m_k = max over d|n of
 *            the k-th digit of d in primorial base (equivalent to
 *            A276085(lcm_{d|n} A276086(d)))
 * Range:     u8 n <= 209, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 97 terms through n = 9660 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const uint64_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t P[16];
    uint64_t maxd[16];
    int i;
    P[0] = 1;
    for (i = 1; i < 16; ++i) P[i] = P[i-1] * pr[i-1];
    for (i = 0; i < 16; ++i) maxd[i] = 0;

    uint64_t N = (uint64_t)n;
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d != 0) continue;
        uint64_t e2 = N / d;
        for (int t = 0; t < 2; ++t) {
            uint64_t x = (t == 0) ? d : e2;
            if (t == 1 && e2 == d) break;
            for (i = 0; i < 16 && P[i] <= x; ++i) {
                uint64_t dig = (x / P[i]) % pr[i];
                if (dig > maxd[i]) maxd[i] = dig;
            }
        }
    }

    seq_t res = 0;
    for (i = 0; i < 16; ++i)
        res += (seq_t)maxd[i] * (seq_t)P[i];
    return res;
}
