/*
 * A386622  a(n) = Sum_{d|n} n^c(d), where c = A351114.
 * https://oeis.org/A386622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + (n-1)*Sum_{d|n} c(d), c = A351114 (c(d)=1 iff
 *            phi(d) | sigma(d))
 * Range:     u8 n <= 59, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A351114_balanced(idx_t n)
{
    if (n == 1) return 1;
    seq_t phi = (seq_t)n;
    seq_t sigma = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0) continue;
        idx_t e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        phi = phi / (seq_t)p * (seq_t)(p - 1);
        seq_t pe = 1;
        for (idx_t i = 0; i < e + 1; ++i) pe = pe * (seq_t)p;
        sigma = sigma * ((pe - 1) / (seq_t)(p - 1));
    }
    if (temp > 1) {
        phi = phi / (seq_t)temp * (seq_t)(temp - 1);
        sigma = sigma * ((seq_t)temp + 1);
    }
    return (sigma % phi) == 0 ? 1 : 0;
}

seq_t a(idx_t n)
{
    seq_t tau = 0;
    seq_t cs = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        tau += 1;
        cs += (seq_t)A351114_balanced(d);
        idx_t d2 = n / d;
        if (d2 != d) {
            tau += 1;
            cs += (seq_t)A351114_balanced(d2);
        }
    }
    return tau + ((seq_t)n - 1) * cs;
}
