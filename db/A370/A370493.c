/*
 * A370493  Numbers k such that A006530(k) = A051903(k).
 * https://oeis.org/A370493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all k <= X whose largest prime factor p
 *            appears with exponent >= 1, all exponents <= p, and at least one
 *            exponent equal to p (so A006530(k) = A051903(k) = p); sort,
 *            doubling X as needed
 * Range:     u8 n <= 8, u16 n <= 61, u32 n <= 1722, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 60 terms through n = 2863 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const int A370493_pr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199};
#define A370493_NP 46

static seq_t *A370493_v = 0;
static size_t A370493_cnt = 0;
static size_t A370493_cap = 0;
static seq_t A370493_X = 0;
static bool A370493_ready = false;
static bool A370493_maxed = false;

static void A370493_push(seq_t x)
{
    if (A370493_cnt == A370493_cap) {
        size_t nc = A370493_cap ? A370493_cap * 2 : 1024;
        A370493_v = mem_resize(seq_t, A370493_v, nc);
        A370493_cap = nc;
    }
    A370493_v[A370493_cnt++] = x;
}

static void A370493_rec(int i, int lo_idx, seq_t val, bool hit, int p, seq_t X)
{
    if (i < 0) {
        if (hit) A370493_push(val);
        return;
    }
    seq_t q = (seq_t)A370493_pr[i];
    int lo = (i == lo_idx) ? 1 : 0;
    seq_t cur = val;
    for (int e = 0; e <= p; e++) {
        if (e >= lo)
            A370493_rec(i - 1, lo_idx, cur, hit || (e == p), p, X);
        if (e == p) break;
        if (cur > X / q) break;
        cur *= q;
    }
}

static void A370493_sort(seq_t *arr, size_t n)
{
    size_t gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (size_t i = gap; i < n; i++) {
            seq_t t = arr[i];
            size_t j = i;
            while (j >= gap && arr[j - gap] > t) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = t;
        }
        if (gap == 1) break;
    }
}

static void A370493_build(seq_t X)
{
    A370493_cnt = 0;
    for (int pi = 0; pi < A370493_NP; pi++) {
        int p = A370493_pr[pi];
        /* need 2^p <= X */
        seq_t v = 1;
        bool ok = true;
        for (int k = 0; k < p; k++) {
            if (v > X / 2) { ok = false; break; }
            v *= 2;
        }
        if (!ok) break;
        A370493_rec(pi, pi, (seq_t)1, false, p, X);
    }
    A370493_sort(A370493_v, A370493_cnt);
    A370493_X = X;
    A370493_ready = true;
}

seq_t a(idx_t n)
{
    if (!A370493_ready)
        A370493_build((seq_t)16);
    while ((idx_t)A370493_cnt < n && !A370493_maxed) {
        seq_t X = A370493_X;
        seq_t Y = X * 2;
        if (Y <= X || Y / 2 != X) {
            /* fixed-width overflow: use the maximum representable value */
            A370493_maxed = true;
            A370493_build((seq_t)0 - 1);
        } else {
            A370493_build(Y);
        }
    }
    if ((idx_t)A370493_cnt < n) return (seq_t)0;
    return A370493_v[(size_t)(n - 1)];
}
