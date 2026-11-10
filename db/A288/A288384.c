/*
 * A288384  a(n) is the n-th term of the leading column of the right triangle
 *          in A274650 minus n.
 * https://oeis.org/A288384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A274650(n,0) - n; A274650 built row by row (least value
 *            absent from row, column, diagonal, antidiagonal) using bitsets
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t u64_a288384;

static size_t A_C = 0, A_W = 0, A_built = 0;
static u64_a288384 *A_col = 0, *A_diag = 0, *A_anti = 0, *A_row = 0;
static long long *A_c0 = 0;

static void A288384_reset(size_t C, size_t W)
{
    free(A_col); free(A_diag); free(A_anti); free(A_row); free(A_c0);
    A_C = C; A_W = W; A_built = 0;
    A_col  = (u64_a288384 *)calloc(C * W, sizeof(u64_a288384));
    A_diag = (u64_a288384 *)calloc(C * W, sizeof(u64_a288384));
    A_anti = (u64_a288384 *)calloc(2 * C * W, sizeof(u64_a288384));
    A_row  = (u64_a288384 *)calloc(W, sizeof(u64_a288384));
    A_c0   = (long long *)calloc(C, sizeof(long long));
}

static int A288384_build_row(size_t r)
{
    size_t W = A_W, k, w;
    memset(A_row, 0, W * sizeof(u64_a288384));
    for (k = 0; k <= r; ++k) {
        u64_a288384 *c = A_col + k * W, *d = A_diag + (r - k) * W, *an = A_anti + (r + k) * W;
        long long v = -1;
        for (w = 0; w < W; ++w) {
            u64_a288384 m = ~(A_row[w] | c[w] | d[w] | an[w]);
            if (m) { v = (long long)(w * 64 + (size_t)__builtin_ctzll(m)); break; }
        }
        if (v < 0) return 0;
        {
            size_t ww = (size_t)v >> 6;
            u64_a288384 b = (u64_a288384)1 << ((size_t)v & 63);
            A_row[ww] |= b; c[ww] |= b; d[ww] |= b; an[ww] |= b;
        }
        if (k == 0) A_c0[r] = v;
    }
    A_built = r + 1;
    return 1;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (A_C == 0 || N >= A_C) {
        size_t C = N + 1 + N / 4 + 64;
        size_t W = A_W ? A_W : 4;
        while (W * 32 < C) W *= 2;
        A288384_reset(C, W);
    }
    while (A_built <= N) {
        if (!A288384_build_row(A_built)) {
            A288384_reset(A_C, A_W * 2);
        }
    }
    return (seq_t)A_c0[N] - (seq_t)n;
}
