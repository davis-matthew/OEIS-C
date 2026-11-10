/*
 * A374799  Irregular table T(n, k), n >= 0, k = 1..A048883(n), read by rows;
 *          the n-th row lists the numbers m such that A003986(m) = n.
 * https://oeis.org/A374799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, tabf
 * Formula:   flat sequence of rows; row r lists, in increasing order, the m
 *            with A003986(m)=r, i.e. m = d(d+1)/2 + x with d = x+y over all
 *            pairs x|y = r (3^popcount(r) of them)
 * Range:     u8 n <= 110, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 83 terms through n = 9090 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int cmp_u64_A374799(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n) {
    uint64_t r = 0;
    uint64_t cum = 0;
    uint64_t size = 1;
    for (;;) {
        uint64_t t = r, w = 0;
        while (t) { t &= t - 1; ++w; }
        size = 1;
        for (uint64_t i = 0; i < w; ++i) size *= 3;
        if ((uint64_t)n < cum + size) break;
        cum += size;
        ++r;
    }
    uint64_t k = (uint64_t)n - cum;

    uint64_t bits[64];
    int w = 0;
    for (int b = 0; b < 64; ++b)
        if ((r >> b) & 1) bits[w++] = (uint64_t)1 << b;

    uint64_t *arr = (uint64_t *)malloc(size * sizeof(uint64_t));
    for (uint64_t t = 0; t < size; ++t) {
        uint64_t u = t, x = 0, y = 0;
        for (int i = 0; i < w; ++i) {
            uint64_t dgt = u % 3;
            u /= 3;
            if (dgt == 0) x |= bits[i];
            else if (dgt == 1) y |= bits[i];
            else { x |= bits[i]; y |= bits[i]; }
        }
        uint64_t d = x + y;
        uint64_t tri = (d % 2 == 0) ? (d / 2) * (d + 1) : d * ((d + 1) / 2);
        arr[t] = tri + x;
    }
    qsort(arr, (size_t)size, sizeof(uint64_t), cmp_u64_A374799);
    uint64_t res = arr[k];
    free(arr);
    return (seq_t)res;
}
