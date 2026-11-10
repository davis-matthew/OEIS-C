/*
 * A353413  a(n) = A000265(A064216(n)).
 * https://oeis.org/A353413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000265(A064216(n)); A064216(n) replaces each prime p in
 *            2n-1 by prevprime(p), and A000265 drops factors of 2 (so p=3 ->
 *            2 contributes nothing)
 * Range:     u8 n <= 131, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 97 terms through n = 9841 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A353413_isp(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A353413_prev(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !A353413_isp(q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    seq_t result = 1;
    if (t <= 1ULL) return result;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            if (p > 3) {
                seq_t q = (seq_t)A353413_prev(p);
                for (int i = 0; i < e; ++i) result *= q;
            }
        }
    }
    if (t > 3) result *= (seq_t)A353413_prev(t);
    return result;
}
