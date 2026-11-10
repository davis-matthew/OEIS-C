/*
 * A317917  Numbers missing from A317915.
 * https://oeis.org/A317917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer not visited by the finite
 *            (2,5)-leaper walk A317915 (found by simulating the walk)
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8779 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t A317917_spiral(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) return 0;
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t r = ax > ay ? ax : ay;
    uint64_t start = (2ull * (uint64_t)r - 1ull) * (2ull * (uint64_t)r - 1ull);
    uint64_t offset;
    if (x == r && y >= -r + 1 && y <= r) {
        offset = (uint64_t)(y + r - 1);
    } else if (y == r && x <= r - 1 && x >= -r) {
        offset = 2ull * (uint64_t)r + (uint64_t)(r - 1 - x);
    } else if (x == -r && y <= r - 1 && y >= -r) {
        offset = 4ull * (uint64_t)r + (uint64_t)(r - 1 - y);
    } else {
        offset = 6ull * (uint64_t)r + (uint64_t)(x + r - 1);
    }
    return start + offset;
}

static uint64_t *A317917_bits = NULL;
static size_t A317917_words = 0;

static void A317917_ensure(size_t words)
{
    if (words <= A317917_words) return;
    size_t nw = A317917_words ? A317917_words << 1 : 4;
    while (nw < words) nw <<= 1;
    uint64_t *nb = (uint64_t *)calloc(nw, sizeof(uint64_t));
    if (A317917_bits) {
        memcpy(nb, A317917_bits, A317917_words * sizeof(uint64_t));
        free(A317917_bits);
    }
    A317917_bits = nb;
    A317917_words = nw;
}

static bool A317917_visited(uint64_t idx)
{
    size_t w = (size_t)(idx >> 6);
    return (w < A317917_words) && ((A317917_bits[w] >> (idx & 63)) & 1ULL);
}

static void A317917_mark(uint64_t idx)
{
    size_t w = (size_t)(idx >> 6);
    A317917_ensure(w + 1);
    A317917_bits[w] |= 1ULL << (idx & 63);
}

seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) {
        static const int dx[8] = { 2,  2, -2, -2,  5,  5, -5, -5 };
        static const int dy[8] = { 5, -5,  5, -5,  2, -2,  2, -2 };
        int64_t cx = 0, cy = 0;
        A317917_ensure(4);
        A317917_bits[0] = 1ULL;
        for (;;) {
            uint64_t best = UINT64_MAX;
            int bk = -1;
            for (int k = 0; k < 8; ++k) {
                uint64_t idx = A317917_spiral(cx + dx[k], cy + dy[k]);
                if (!A317917_visited(idx) && idx < best) {
                    best = idx;
                    bk = k;
                }
            }
            if (bk < 0) break;
            cx += dx[bk];
            cy += dy[bk];
            A317917_mark(best);
        }
        init = true;
    }

    uint64_t value = (uint64_t)n - 1;
    for (size_t w = 0; w < A317917_words; ++w) {
        uint64_t word = A317917_bits[w];
        while (word) {
            int b = 0;
            while (!((word >> b) & 1ULL)) ++b;
            word &= word - 1;
            uint64_t v = ((uint64_t)w << 6) + (uint64_t)b;
            if (v <= value) value++;
            else return (seq_t)value;
        }
    }
    return (seq_t)value;
}
