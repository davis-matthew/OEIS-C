/*
 * A353372  Inverse Möbius transform of A353370.
 * https://oeis.org/A353372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A353370(d) = number of divisors d of n with as
 *            many even as odd prime indices (with multiplicity); computed by
 *            a DP over the prime factorization of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8770 (2026-11-10)
 * Helpers:   prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/prime_pi_u64.h"

seq_t a(idx_t n)
{
    enum { OFF = 70, SZ = 141 };
    uint64_t cur[SZ], nxt[SZ];
    uint64_t m = (uint64_t)n;
    int i, k;
    for (i = 0; i < SZ; i++) cur[i] = 0;
    cur[OFF] = 1;

    uint64_t p = 2;
    while (m > 1) {
        uint64_t q;
        unsigned e = 0;
        if (p > m / p) {
            q = m;          /* remaining prime */
        } else if (m % p == 0) {
            q = p;
        } else {
            p += (p == 2) ? 1 : 2;
            continue;
        }
        while (m % q == 0) { m /= q; e++; }
        uint64_t idx = prime_pi_u64(q);
        int s = (idx & 1) ? 1 : -1;
        for (i = 0; i < SZ; i++) nxt[i] = 0;
        for (i = 0; i < SZ; i++) {
            if (!cur[i]) continue;
            for (k = 0; k <= (int)e; k++) {
                int j = i + s * k;
                if (j >= 0 && j < SZ) nxt[j] += cur[i];
            }
        }
        for (i = 0; i < SZ; i++) cur[i] = nxt[i];
        p = q + ((q == 2) ? 1 : 2);
    }
    return (seq_t)cur[OFF];
}
