/*
 * A331306  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A285732(i) = A285732(j) for all i, j.
 * https://oeis.org/A331306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = restricted growth sequence transform of A285732 (labels
 *            numbered 1,2,... in order of first appearance of each distinct
 *            value A285732(i)); computed by hashing
 * Range:     u8 n <= 376, u16 n <= 25425, u32 n <= 25425, u64 n <= 25425,
 *            u128 n <= 25425, big any n
 * Validated: standard, pass, 116 terms through n = 9974 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A331306_lab = 0;
static idx_t A331306_cap = 0;

static void A331306_build(idx_t N)
{
    idx_t hs = 1;
    while (hs < 2 * N + 2) hs <<= 1;
    uint64_t *keys = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)hs);
    uint64_t *vals = (uint64_t *)calloc((size_t)hs, sizeof(uint64_t));
    uint64_t *lab = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)N);
    idx_t m = 0;
    uint64_t next = 1;
    uint64_t d = 1;
    while (m < N) {
        for (uint64_t off = 0; off < d && m < N; off++) {
            uint64_t row = off + 1, col = d - off;
            uint64_t key;
            if (row == col) {
                key = 2 * row + 1;
            } else {
                uint64_t u, v;
                if (row > col) { u = row - col; v = col; }
                else { u = row; v = col - row; }
                uint64_t s = u + v;
                key = 2 * ((s - 2) * (s - 1) / 2 + u);
            }
            idx_t h = (idx_t)((key * 0x9E3779B97F4A7C15ULL) >> 20) & (hs - 1);
            while (vals[h] != 0 && keys[h] != key) h = (h + 1) & (hs - 1);
            if (vals[h] == 0) {
                keys[h] = key;
                vals[h] = next++;
            }
            lab[m++] = vals[h];
        }
        d++;
    }
    free(keys);
    free(vals);
    free(A331306_lab);
    A331306_lab = lab;
    A331306_cap = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A331306_cap) {
        idx_t N = n < 4096 ? 4096 : n;
        if (N < 2 * A331306_cap && 2 * A331306_cap <= 4000000) N = 2 * A331306_cap;
        A331306_build(N);
    }
    return (seq_t)A331306_lab[n - 1];
}
