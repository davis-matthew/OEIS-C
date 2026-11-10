/*
 * A389897  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007814(i) = A007814(j), A007949(i) = A007949(j), A343029(i) =
 *          A343029(j) and A343030(i) = A343030(j) for all i, j >= 1.
 * https://oeis.org/A389897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of [A007814(n), A007949(n),
 *            A343029(n), A343030(n)]; computed by scanning 1..n and numbering
 *            each new quadruple in order of first appearance
 * Range:     u8 n <= 983, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 105 terms through n = 9809 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A389897_key(uint64_t n) {
    uint64_t v2 = 0, v3 = 0, e = 0, o = 0, zeros = 0, t, m = n;
    while ((m & 1) == 0) { v2++; m >>= 1; }
    m = n;
    while (m % 3 == 0) { v3++; m /= 3; }
    t = n;
    while (t) {
        if (t & 1) {
            if (zeros & 1) o++; else e++;
        } else {
            zeros++;
        }
        t >>= 1;
    }
    return (v2 << 24) | (v3 << 16) | (e << 8) | o;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t size = 16, i, label = 0, result = 0;
    while (size < 2 * N + 2) size <<= 1;
    uint64_t *keys = (uint64_t *)malloc(size * sizeof(uint64_t));
    uint64_t *vals = (uint64_t *)malloc(size * sizeof(uint64_t));
    unsigned char *used = (unsigned char *)calloc(size, 1);
    for (i = 1; i <= N; i++) {
        uint64_t k = A389897_key(i);
        uint64_t h = (k * 0x9E3779B97F4A7C15ULL) >> 20;
        h &= size - 1;
        while (used[h] && keys[h] != k) h = (h + 1) & (size - 1);
        if (!used[h]) {
            used[h] = 1;
            keys[h] = k;
            vals[h] = ++label;
        }
        result = vals[h];
    }
    free(keys);
    free(vals);
    free(used);
    return (seq_t)result;
}
