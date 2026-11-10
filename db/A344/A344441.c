/*
 * A344441  a(n) = A061020(n) + abs(A061020(n)).
 * https://oeis.org/A344441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A061020(n) + abs(A061020(n)); A061020(n) = Prod_{p^e ||
 *            n} (1-(-p)^(e+1))/(p+1). Returns 2*A061020(n) if it is positive,
 *            otherwise 0
 * Range:     u8 n <= 48, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9595 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t prod = 1;
    int neg = 0;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }
            seq_t pw = 1;
            unsigned k;
            for (k = 0; k < e + 1; ++k) pw *= (seq_t)p;
            seq_t num;
            if (e & 1u) { num = pw - 1; neg ^= 1; }
            else num = pw + 1;
            prod *= num / ((seq_t)p + 1);
        }
    }
    if (m > 1) {
        /* exponent 1: |factor| = (m^2 - 1)/(m+1) = m - 1, negative sign */
        prod *= ((seq_t)m - 1);
        neg ^= 1;
    }
    if (neg) return (seq_t)0;
    return prod * 2;
}
