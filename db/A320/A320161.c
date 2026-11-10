/*
 * A320161  Irregular triangle read by rows: row n lists 0 <= k < p^2 such
 *          that p^2 divides A316269(k, p-Kronecker(k^2-4, p)), p = prime(n).
 * https://oeis.org/A320161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n (p = prime(n)) lists 0 <= k < p^2 with p^2 | U_m(k), where
 *            U_0=0, U_1=1, U_j = k*U_{j-1} - U_{j-2} (A316269) and m = p -
 *            Kronecker(k^2-4, p). Computed by direct search over k; row
 *            lengths are 3 for p=2 and p=3, and p-2 for p>=5
 * Range:     u8 n <= 47, u16 n <= 6229, u32 n <= 29083, u64 n <= 29083,
 *            u128 n <= 29083, big any n
 * Validated: standard, pass, 82 terms through n = 9110 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int a_isprime_small(uint64_t x)
{
    if (x < 2) return 0;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t rem = (uint64_t)n;   /* 1-based position */
    uint64_t p = 2;
    for (;;) {
        uint64_t len = (p == 2 || p == 3) ? 3 : p - 2;
        if (rem <= len) break;
        rem -= len;
        do { p++; } while (!a_isprime_small(p));
    }
    uint64_t M = p * p;
    unsigned char *sq = (unsigned char *)malloc((size_t)p + 1);
    for (uint64_t i = 0; i < p; i++) sq[i] = 0;
    if (p > 2)
        for (uint64_t i = 1; i < p; i++) sq[(i * i) % p] = 1;
    uint64_t count = 0;
    uint64_t result = 0;
    for (uint64_t k = 0; k < M; k++) {
        int chi;
        if (p == 2) {
            chi = (k & 1) ? -1 : 0;
        } else {
            uint64_t km = k % p;
            uint64_t d = (km * km + p - (4 % p)) % p;
            if (d == 0) chi = 0;
            else chi = sq[d] ? 1 : -1;
        }
        uint64_t m = p - (uint64_t)0 - 0;
        if (chi == 1) m = p - 1;
        else if (chi == -1) m = p + 1;
        uint64_t u0 = 0, u1 = 1;
        for (uint64_t i = 1; i < m; i++) {
            uint64_t c = (k * u1) % M;
            c = (c + M - u0) % M;
            u0 = u1;
            u1 = c;
        }
        if (u1 % M == 0) {
            count++;
            if (count == rem) { result = k; break; }
        }
    }
    free(sq);
    return (seq_t)result;
}
