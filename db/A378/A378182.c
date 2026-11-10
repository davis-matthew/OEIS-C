/*
 * A378182  Sum of row n of A378180.
 * https://oeis.org/A378182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all m whose prime divisors all divide n and with
 *            bigomega(m) <= bigomega(n)-1 (a(1)=0); computed via
 *            complete-homogeneous-polynomial DP over the distinct primes of n
 * Range:     u8 n <= 47, u16 n <= 271, u32 n <= 2623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9540 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t S[70];
    uint64_t primes[70];
    int cnt = 0, big = 0;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    if (n <= 1) return 0;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            primes[cnt++] = p;
            while (m % p == 0) { m /= p; big++; }
        }
    }
    if (m > 1) { primes[cnt++] = m; big++; }

    int D = big - 1;
    for (int d = 0; d <= D; d++) S[d] = 0;
    S[0] = 1;
    for (int i = 0; i < cnt; i++) {
        seq_t q = (seq_t)primes[i];
        for (int d = 1; d <= D; d++) S[d] += q * S[d - 1];
    }
    seq_t total = 0;
    for (int d = 0; d <= D; d++) total += S[d];
    return total;
}
