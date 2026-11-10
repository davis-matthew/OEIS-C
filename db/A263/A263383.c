/*
 * A263383  Lengths of cycles of permutation A263327.
 * https://oeis.org/A263383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = length of the n-th cycle of permutation A263327
 *            (decreasing-digit numbers, numeric rank -> lexicographic rank),
 *            cycles ordered by their largest element; computed by building
 *            the permutation of {0..1023} and decomposing it into cycles
 * Range:     u8 n <= 14, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A263383_N 1024

static unsigned long long A263383_vals[A263383_N];
static char A263383_str[A263383_N][32];
static int A263383_numidx[A263383_N];
static int A263383_lexidx[A263383_N];
static int A263383_perm[A263383_N];
static int A263383_cid[A263383_N];
static int A263383_cmax[A263383_N];
static int A263383_clen[A263383_N];
static int A263383_len[A263383_N];
static int A263383_cnt = 0;
static bool A263383_init = false;

static int A263383_cmp_num(const void *x, const void *y)
{
    int i = *(const int *)x, j = *(const int *)y;
    if (A263383_vals[i] < A263383_vals[j]) return -1;
    if (A263383_vals[i] > A263383_vals[j]) return 1;
    return (i > j) - (i < j);
}

static int A263383_cmp_lex(const void *x, const void *y)
{
    int i = *(const int *)x, j = *(const int *)y;
    int c = strcmp(A263383_str[i], A263383_str[j]);
    if (c) return c;
    return (i > j) - (i < j);
}

static void A263383_setup(void)
{
    int i, d, mask, pos, ncyc = 0;
    static int numrank[A263383_N];
    if (A263383_init) return;
    for (mask = 0; mask < A263383_N; ++mask) {
        unsigned long long v = 0;
        for (d = 9; d >= 0; --d)
            if (mask & (1 << d)) v = v * 10 + (unsigned long long)d;
        A263383_vals[mask] = v;
        snprintf(A263383_str[mask], sizeof(A263383_str[mask]), "%llu", v);
        A263383_numidx[mask] = mask;
        A263383_lexidx[mask] = mask;
    }
    qsort(A263383_numidx, A263383_N, sizeof(int), A263383_cmp_num);
    qsort(A263383_lexidx, A263383_N, sizeof(int), A263383_cmp_lex);
    for (pos = 0; pos < A263383_N; ++pos) numrank[A263383_numidx[pos]] = pos;
    for (pos = 0; pos < A263383_N; ++pos)
        A263383_perm[numrank[A263383_lexidx[pos]]] = pos;

    for (i = 0; i < A263383_N; ++i) A263383_cid[i] = -1;
    for (i = 0; i < A263383_N; ++i) {
        if (A263383_cid[i] < 0) {
            int l = 0, x = i, mx = i;
            while (A263383_cid[x] < 0) {
                A263383_cid[x] = ncyc;
                if (x > mx) mx = x;
                x = A263383_perm[x];
                ++l;
            }
            A263383_clen[ncyc] = l;
            A263383_cmax[ncyc] = mx;
            ++ncyc;
        }
    }
    /* order cycles by their largest element */
    for (i = 0; i < A263383_N; ++i) {
        int c = A263383_cid[i];
        if (A263383_cmax[c] == i) A263383_len[A263383_cnt++] = A263383_clen[c];
    }
    A263383_init = true;
}

seq_t a(idx_t n)
{
    A263383_setup();
    if (n < 1 || n > (idx_t)A263383_cnt) return (seq_t)0;
    return (seq_t)A263383_len[(int)(n - 1)];
}
