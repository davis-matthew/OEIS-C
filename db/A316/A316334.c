/*
 * A316334  Same as A316588, except number the squares starting at 0 rather
 *          than 1.
 * https://oeis.org/A316334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, look
 * Formula:   a(n) = A316588(n+1) - 1; A316588 is a greedy knight walk on a
 *            diagonally numbered quadrant board, starting at square 1 and
 *            always moving to the lowest-numbered unvisited square. The code
 *            finds the terms by simulating the walk
 * Range:     u8 n <= 139, u16 n <= 2401, u32 n <= 2401, u64 n <= 2401,
 *            u128 n <= 2401, big any n
 * Validated: standard, pass, 90 terms through n = 2398 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A316334_vals = NULL;
static int64_t *A316334_xs = NULL;
static int64_t *A316334_ys = NULL;
static size_t A316334_len = 0;
static bool A316334_done = false;

static seq_t A316334_board(int64_t x, int64_t y)
{
    seq_t d = (seq_t)(x + y - 2);
    return d * (d + 1) / 2 + d + 2 - (seq_t)x;
}

static void A316334_build(void)
{
    static const int dx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    static const int dy[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    size_t cap = 4096;
    A316334_vals = mem_new(seq_t, cap);
    A316334_xs = (int64_t *)malloc(cap * sizeof(int64_t));
    A316334_ys = (int64_t *)malloc(cap * sizeof(int64_t));
    if (!A316334_xs || !A316334_ys) abort();
    A316334_vals[0] = (seq_t)1;
    A316334_xs[0] = 1;
    A316334_ys[0] = 1;
    A316334_len = 1;
    for (;;) {
        int64_t cx0 = A316334_xs[A316334_len - 1];
        int64_t cy0 = A316334_ys[A316334_len - 1];
        bool found = false;
        seq_t best = 0;
        int64_t bx = 0, by = 0;
        for (int i = 0; i < 8; ++i) {
            int64_t cx = cx0 + dx[i], cy = cy0 + dy[i];
            if (cx < 1 || cy < 1) continue;
            bool vis = false;
            for (size_t k = 0; k < A316334_len; ++k) {
                if (A316334_xs[k] == cx && A316334_ys[k] == cy) { vis = true; break; }
            }
            if (vis) continue;
            seq_t c = A316334_board(cx, cy);
            if (!found || c < best) {
                found = true; best = c; bx = cx; by = cy;
            }
        }
        if (!found) break;
        if (A316334_len >= cap) {
            size_t ncap = cap * 2;
            seq_t *nv = mem_resize(seq_t, A316334_vals, ncap);
            if (!nv) abort();
            A316334_vals = nv;
            int64_t *nx = (int64_t *)realloc(A316334_xs, ncap * sizeof(int64_t));
            int64_t *ny = (int64_t *)realloc(A316334_ys, ncap * sizeof(int64_t));
            if (!nx || !ny) abort();
            A316334_xs = nx;
            A316334_ys = ny;
            cap = ncap;
        }
        A316334_vals[A316334_len] = best;
        A316334_xs[A316334_len] = bx;
        A316334_ys[A316334_len] = by;
        ++A316334_len;
    }
    A316334_done = true;
}

seq_t a(idx_t n)
{
    if (!A316334_done) A316334_build();
    if ((uint64_t)n >= (uint64_t)A316334_len) return (seq_t)0;
    return A316334_vals[(size_t)n] - 1;
}
