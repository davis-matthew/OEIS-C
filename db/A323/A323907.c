/*
 * A323907  Lexicographically earliest positive sequence such that a(i) = a(j)
 *          => A004718(i) = A004718(j), for all i, j >= 0.
 * https://oeis.org/A323907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A004718, where
 *            A004718(2n) = -A004718(n), A004718(2n+1) = A004718(n)+1,
 *            A004718(0) = 0; computed by scanning i = 0..n and numbering
 *            distinct values in order of first appearance
 * Range:     u8 n <= 65535, u16 n <= 65535, u32 n <= 65535, u64 n <= 65535,
 *            u128 n <= 65535, big any n
 * Validated: standard, pass, 122 terms through n = 8395 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A004718_val(idx_t n) {
    int r = 0;
    int msb = -1;
    idx_t t = n;
    while (t > 0) { msb++; t >>= 1; }
    for (int i = msb; i >= 0; i--) {
        if ((n >> i) & 1) r += 1;
        else r = -r;
    }
    return r;
}

seq_t a(idx_t n) {
    int rank[256];
    for (int k = 0; k < 256; k++) rank[k] = 0;
    int cnt = 0;
    int res = 0;
    for (idx_t i = 0; i <= n; i++) {
        int v = A004718_val(i) + 128;
        if (rank[v] == 0) rank[v] = ++cnt;
        if (i == n) res = rank[v];
    }
    return (seq_t)res;
}
