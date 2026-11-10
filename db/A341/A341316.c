/*
 * A341316  Row sums in A341315.
 * https://oeis.org/A341316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(A057660(n)+1)/2 for n>=1, a(0)=0; A057660(n) =
 *            Sum_{d|n} d*phi(d), multiplicative with a(p^e) = Sum_{i=0..e}
 *            p^i*phi(p^i)
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    seq_t S = 1;
    uint64_t p = 2;
    while (p <= m / p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            seq_t term = 1;
            seq_t pk = 1;          /* p^i */
            seq_t phi = 1;         /* phi(p^i) */
            for (int i = 1; i <= e; ++i) {
                pk *= (seq_t)p;
                phi = (i == 1) ? (seq_t)(p - 1) : phi * (seq_t)p;
                term += pk * phi;
            }
            S *= term;
        }
        p += (p == 2) ? 1 : 2;
    }
    if (m > 1) {
        S *= (seq_t)1 + (seq_t)m * (seq_t)(m - 1);
    }
    return (S + 1) / 2 * 3;
}
