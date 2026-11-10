/*
 * A319349  Filter sequence combining parity of n, A003557(n) and A051953(n).
 * https://oeis.org/A319349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the triple [n mod
 *            2, A003557(n), A051953(n)]; computed by scanning m = 1..n and
 *            numbering distinct triples in order of first appearance
 * Range:     u8 n <= 381, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9957 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A319349_f(uint64_t m, uint64_t *pw, uint64_t *cot)
{
    uint64_t n = m, w = 1, phi = m;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; e++; }
            for (uint64_t i = 1; i < e; i++) w *= p;
            phi -= phi / p;
        }
    }
    if (n > 1) phi -= phi / n;
    *pw = w;
    *cot = m - phi;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t size = 16;
    while (size < 2 * N + 2) size <<= 1;
    uint64_t *k1 = (uint64_t *)calloc(size, sizeof(uint64_t));
    uint64_t *k2 = (uint64_t *)calloc(size, sizeof(uint64_t));
    uint64_t *lb = (uint64_t *)calloc(size, sizeof(uint64_t)); /* 0 = empty */
    uint64_t next = 1, result = 0;
    for (uint64_t m = 1; m <= N; m++) {
        uint64_t w, c;
        A319349_f(m, &w, &c);
        uint64_t x = c * 2 + (m & 1), y = w;
        uint64_t h = (x * 0x9E3779B97F4A7C15ULL) ^ (y * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        uint64_t i = h & (size - 1);
        uint64_t label = 0;
        while (lb[i]) {
            if (k1[i] == x && k2[i] == y) { label = lb[i]; break; }
            i = (i + 1) & (size - 1);
        }
        if (!label) {
            label = next++;
            k1[i] = x; k2[i] = y; lb[i] = label;
        }
        if (m == N) result = label;
    }
    free(k1); free(k2); free(lb);
    return (seq_t)result;
}
