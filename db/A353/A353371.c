/*
 * A353371  Number of divisors of n that are not in A325698.
 * https://oeis.org/A353371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (1 - A353370(d)); computed via DP over the
 *            prime factorization, counting divisors whose odd-index and
 *            even-index prime counts differ
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8774 (2026-11-10)
 * Helpers:   prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/prime_pi_u64.h"

seq_t a(idx_t n)
{
    enum { OFF = 70, SZ = 141 };
    uint64_t cnt[SZ], nxt[SZ];
    int lo = OFF, hi = OFF;
    uint64_t tau = 1;
    uint64_t m = (uint64_t)n;
    int i;

    for (i = 0; i < SZ; i++) cnt[i] = 0;
    cnt[OFF] = 1;

    uint64_t p = 2;
    while (m > 1) {
        if (p > m / p) p = m; /* remaining m is prime */
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            int s = (prime_pi_u64(p) & 1) ? 1 : -1;
            for (i = 0; i < SZ; i++) nxt[i] = 0;
            for (i = lo; i <= hi; i++) {
                if (!cnt[i]) continue;
                for (int k = 0; k <= e; k++)
                    nxt[i + s * k] += cnt[i];
            }
            if (s > 0) hi += e; else lo -= e;
            for (i = 0; i < SZ; i++) cnt[i] = nxt[i];
            tau *= (uint64_t)(e + 1);
        }
        p++;
    }
    return (seq_t)(tau - cnt[OFF]);
}
