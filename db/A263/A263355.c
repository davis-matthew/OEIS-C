/*
 * A263355  Table read by rows: cycles of the permutation A263327, sorted in
 *          increasing order of their largest element. The elements in each
 *          cycle are listed in decreasing numerical order.
 * https://oeis.org/A263355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   cycles of the permutation A263327 on {0..1023}, listed by
 *            increasing largest element, each cycle in decreasing order;
 *            computed by direct cycle decomposition
 * Range:     u8 n <= 19, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: full, pass, 1024 terms through n = 1024 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A263355_MAXV 1024

static unsigned long long A263355_values[A263355_MAXV];
static char A263355_strbuf[A263355_MAXV][32];
static int A263355_perm[A263355_MAXV];
static int A263355_flat[A263355_MAXV];
static bool A263355_ready = false;

static int A263355_cmp_num(const void *x, const void *y)
{
    int i = *(const int *)x, j = *(const int *)y;
    if (A263355_values[i] < A263355_values[j]) return -1;
    if (A263355_values[i] > A263355_values[j]) return 1;
    return 0;
}

static int A263355_cmp_lex(const void *x, const void *y)
{
    int i = *(const int *)x, j = *(const int *)y;
    return strcmp(A263355_strbuf[i], A263355_strbuf[j]);
}

static int A263355_cmp_desc(const void *x, const void *y)
{
    int i = *(const int *)x, j = *(const int *)y;
    return (j > i) - (j < i);
}

static void A263355_init(void)
{
    int num_idx[A263355_MAXV], lex_idx[A263355_MAXV], numrank[A263355_MAXV];
    int cyc[A263355_MAXV];
    int mask, d, i, m, pos, len, k, x;
    if (A263355_ready) return;

    for (mask = 0; mask < A263355_MAXV; ++mask) {
        unsigned long long val = 0;
        for (d = 9; d >= 0; --d)
            if (mask & (1 << d)) val = val * 10 + (unsigned long long)d;
        A263355_values[mask] = val;
        snprintf(A263355_strbuf[mask], sizeof(A263355_strbuf[mask]), "%llu", val);
        num_idx[mask] = mask;
        lex_idx[mask] = mask;
    }
    qsort(num_idx, A263355_MAXV, sizeof(int), A263355_cmp_num);
    qsort(lex_idx, A263355_MAXV, sizeof(int), A263355_cmp_lex);
    for (pos = 0; pos < A263355_MAXV; ++pos) numrank[num_idx[pos]] = pos;
    for (pos = 0; pos < A263355_MAXV; ++pos)
        A263355_perm[numrank[lex_idx[pos]]] = pos;

    pos = 0;
    for (m = 0; m < A263355_MAXV; ++m) {
        bool is_max = true;
        len = 0;
        x = m;
        do {
            if (x > m) { is_max = false; break; }
            cyc[len++] = x;
            x = A263355_perm[x];
        } while (x != m);
        if (!is_max) continue;
        qsort(cyc, (size_t)len, sizeof(int), A263355_cmp_desc);
        for (k = 0; k < len; ++k) A263355_flat[pos++] = cyc[k];
    }
    (void)i;
    A263355_ready = true;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > A263355_MAXV) return (seq_t)0;
    A263355_init();
    return (seq_t)A263355_flat[(int)(n - 1)];
}
