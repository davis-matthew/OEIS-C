/*
 * A322318  a(n) = gcd(A003557(n), A048250(n)).
 * https://oeis.org/A322318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003557(n), A048250(n)), where A003557(n) = n/rad(n)
 *            and A048250(n) = Product_{p|n} (p+1)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9761 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A322318_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t u = 1;   /* A003557 */
    seq_t s = 1;   /* A048250 */

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            for (uint64_t i = 1; i < e; i++)
                u *= (seq_t)p;
            s *= ((seq_t)p + 1);
        }
    }
    if (m > 1)
        s *= ((seq_t)m + 1);

    return A322318_gcd(u, s);
}
