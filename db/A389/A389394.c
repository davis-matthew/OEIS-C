/*
 * A389394  Powers k^m, m > 1, where k is neither squarefree nor squareful and
 *          A006530(k) < A053669(k).
 * https://oeis.org/A389394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate k = Product_{i=1..r} prime(i)^e_i with r >= 2,
 *            min(e_i) = 1, max(e_i) >= 2, and output all k^m <= X for m >= 2,
 *            sorted; X doubles until n terms are found (it is capped at the
 *            seq_t maximum instead of overflowing)
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 274, u64 n <= 9459,
 *            u128 n <= 18030, big any n
 * Validated: standard, pass, 53 terms through n = 9840 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A389394_NP 40

static unsigned A389394_pr[A389394_NP];
static seq_t *A389394_arr = NULL;
static size_t A389394_cnt = 0, A389394_cap = 0;
static seq_t A389394_X = 0;
static bool A389394_init = false;
static bool A389394_atmax = false;
static bool A389394_built = false;

static void A389394_push(seq_t v)
{
    if (A389394_cnt == A389394_cap) {
        size_t nc = A389394_cap ? A389394_cap * 2 : 1024;
        if (A389394_arr == NULL) A389394_arr = mem_new(seq_t, nc);
        else A389394_arr = mem_resize(seq_t, A389394_arr, nc);
        A389394_cap = nc;
    }
    A389394_arr[A389394_cnt++] = v;
}

static bool A389394_mulpow(seq_t *w, unsigned p, int m)
{
    for (int j = 0; j < m; j++) {
        if (*w > A389394_X / (seq_t)p) return false;
        *w = *w * (seq_t)p;
    }
    return true;
}

static void A389394_dfs(int idx, seq_t v, int mn, int mx, int m)
{
    if (idx >= 2 && mn == 1 && mx >= 2) A389394_push(v);
    if (idx >= A389394_NP) return;
    unsigned p = A389394_pr[idx];
    seq_t w = v;
    int f = 0;
    while (A389394_mulpow(&w, p, m)) {
        f++;
        A389394_dfs(idx + 1, w, f < mn ? f : mn, f > mx ? f : mx, m);
    }
}

static void A389394_sort(seq_t *a, size_t n)
{
    size_t gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (size_t i = gap; i < n; i++) {
            seq_t t = a[i];
            size_t j = i;
            while (j >= gap && a[j - gap] > t) {
                a[j] = a[j - gap];
                j -= gap;
            }
            a[j] = t;
        }
        if (gap == 1) break;
    }
}

static void A389394_build(void)
{
    A389394_cnt = 0;
    for (int m = 2; ; m++) {
        seq_t t = 1;
        bool ok = true;
        for (int j = 0; j < m; j++) {
            if (t > A389394_X / 12) { ok = false; break; }
            t = t * 12;
        }
        if (!ok) break;
        A389394_dfs(0, (seq_t)1, 1000000, 0, m);
    }
    A389394_sort(A389394_arr, A389394_cnt);
    A389394_built = true;
}

seq_t a(idx_t n)
{
    if (!A389394_init) {
        int c = 0;
        for (unsigned x = 2; c < A389394_NP; x++) {
            bool ok = true;
            for (unsigned d = 2; d * d <= x; d++)
                if (x % d == 0) { ok = false; break; }
            if (ok) A389394_pr[c++] = x;
        }
        A389394_X = 144;
        A389394_init = true;
        A389394_cnt = 0;
        A389394_built = false;
    }
    if (n == 0) return (seq_t)0;
    if (!A389394_built) A389394_build();
    while ((idx_t)A389394_cnt < n) {
        if (A389394_atmax) return (seq_t)0;
        seq_t nx = A389394_X * 2;
        if (nx < A389394_X || nx / 2 != A389394_X) {
            /* doubling overflowed a fixed-width seq_t: use its maximum value */
            nx = (seq_t)0 - (seq_t)1;
            A389394_atmax = true;
        }
        A389394_X = nx;
        A389394_build();
    }
    return A389394_arr[(size_t)(n - 1)];
}
