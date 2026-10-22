/*
 * A384039  The number of integers k from 1 to n such that gcd(n,k) is a
 *          powerful number.
 * https://oeis.org/A384039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Σ_{d|n, d squarefull} φ(n/d) Multiplicative: for n = Π
 *            p^e, a(p^e) = { p-1 if e = 1 { (p^2 - p + 1) * p^(e-2) if e ≥ 2
 *            } The function below implements this directly
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Handle prime factor 2 separately. */
    unsigned int e = 0;
    while ((m & 1ULL) == 0) { m >>= 1; ++e; }
    if (e > 0) {
        if (e == 1) {
            result *= ((seq_t)2 - 1);
        } else {
            seq_t base = ((seq_t)2 * (seq_t)2) - (seq_t)2 + (seq_t)1; /* 2^2 - 2 + 1 */
            result *= base * power_uint64(2, e - 2);
        }
    }

    /* Process odd prime factors up to sqrt(m). */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0) continue;
        e = 0;
        while (m % p == 0) { m /= p; ++e; }
        if (e == 1) {
            result *= ((seq_t)p - 1);
        } else {
            seq_t base = ((seq_t)p * (seq_t)p) - (seq_t)p + (seq_t)1; /* p^2 - p + 1 */
            result *= base * power_uint64(p, e - 2);
        }
    }

    /* Remaining factor > 1 is a prime. */
    if (m > 1) {
        result *= ((seq_t)m - 1);
    }

    return result;
}
