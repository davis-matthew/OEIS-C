/*
 * A351883  Expansion of e.g.f. 1 / (1 - x)^sech(x).
 * https://oeis.org/A351883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{k=1..n} (-1)^k * binomial(n-1,k-1) * A009435(k) *
 *            a(n-k), a(0) = 1; A009435 = e.g.f. log(1+x)/cosh(x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A009435_g = NULL;
static seq_t *A009435_arr = NULL;
static size_t A009435_cap = 0;
static idx_t A009435_done = 0;

static void A009435_ensure(size_t need)
{
    if (A009435_cap >= need) return;
    size_t nc = A009435_cap ? A009435_cap * 2 : 1;
    while (nc < need) nc <<= 1;
    A009435_g   = mem_resize(seq_t, A009435_g, nc);
    A009435_arr = mem_resize(seq_t, A009435_arr, nc);
    A009435_cap = nc;
}

static seq_t A009435_a(idx_t n)
{
    if (A009435_done == 0) {
        A009435_ensure(1);
        A009435_g[0] = 1;
        A009435_arr[0] = 0;
    }
    if (n > A009435_done) {
        A009435_ensure((size_t)n + 1);
        for (idx_t m = A009435_done + 1; m <= n; ++m) {
            seq_t sum_g = 0;
            seq_t bg = 1;
            idx_t max_i = m >> 1;
            for (idx_t i = 1; i <= max_i; ++i) {
                bg = (bg * (seq_t)(m - 2*i + 2) * (seq_t)(m - 2*i + 1)) /
                     ((seq_t)(2*i) * (seq_t)(2*i - 1));
                sum_g += bg * A009435_g[m - 2*i];
            }
            A009435_g[m] = -sum_g;

            seq_t sum_a = 0;
            seq_t ba = 1;
            seq_t h = 1;
            for (idx_t k = 1; k <= m; ++k) {
                ba = (ba * (seq_t)(m - k + 1)) / (seq_t)k;
                seq_t term = ba * h;
                term *= A009435_g[m - k];
                sum_a += term;
                h = -(seq_t)k * h;
            }
            A009435_arr[m] = sum_a;
        }
        A009435_done = n;
    }
    return A009435_arr[n];
}

static seq_t *memo_a = NULL;
static size_t memo_cap = 0;
static idx_t memo_done = 0;
static int memo_init = 0;

seq_t a(idx_t n)
{
    if (!memo_init) {
        memo_cap = 1;
        memo_a = mem_resize(seq_t, memo_a, memo_cap);
        memo_a[0] = 1;
        memo_done = 0;
        memo_init = 1;
    }
    if (n > memo_done) {
        if (memo_cap < (size_t)n + 1) {
            size_t nc = memo_cap;
            while (nc < (size_t)n + 1) nc <<= 1;
            memo_a = mem_resize(seq_t, memo_a, nc);
            memo_cap = nc;
        }
        for (idx_t m = memo_done + 1; m <= n; ++m) {
            seq_t sum = 0;
            seq_t c = 1; /* C(m-1, k-1) */
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = c * A009435_a(k);
                term *= memo_a[m - k];
                if (k & 1) sum += term; else sum -= term;
                /* C(m-1,k) = C(m-1,k-1)*(m-k)/k */
                c = (c * (seq_t)(m - k)) / (seq_t)k;
            }
            memo_a[m] = sum;
        }
        memo_done = n;
    }
    return memo_a[n];
}
