/*
 * A300790  Numbers k the smallest solution of A023900(m) = s, m >= 1, s <> 0,
 *          such that A023900(k) is negative and k is nonprime.
 * https://oeis.org/A300790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over squarefree k with an odd number (>= 3) of prime
 *            factors. k is a term if no smaller squarefree m with an odd
 *            number of prime factors has Product_{p|m} (p-1) = Product_{p|k}
 *            (p-1), i.e. A023900(m) = A023900(k) < 0
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A300790_isprime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

/* Is there a squarefree m < K, built from primes in ascending order above `last`,
   with Product (p-1) == T and an odd number of prime factors? */
static bool A300790_dfs(uint64_t K, uint64_t last, uint64_t prod, uint64_t T, int par)
{
    uint64_t p;
    if (T == 1 && par == 1 && prod > 1) return true;
    p = last + 1;
    if (p < 2) p = 2;
    for (; p <= T + 1 && prod * p < K; p++) {
        if (T % (p - 1) != 0) continue;
        if (!A300790_isprime(p)) continue;
        if (A300790_dfs(K, p, prod * p, T / (p - 1), par ^ 1)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t k;
    idx_t count = 0;
    for (k = 2;; k++) {
        uint64_t m = k, T = 1, p;
        int w = 0;
        bool ok = true;
        for (p = 2; p * p <= m; p++) {
            if (m % p == 0) {
                m /= p;
                if (m % p == 0) { ok = false; break; }
                T *= (p - 1);
                w++;
            }
        }
        if (!ok) continue;
        if (m > 1) { T *= (m - 1); w++; }
        if ((w & 1) == 0 || w < 3) continue;
        if (A300790_dfs(k, 0, 1, T, 0)) continue;
        if (++count == n) return (seq_t)k;
    }
}
