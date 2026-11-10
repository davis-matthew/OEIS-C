/*
 * A258049  Position of n in A258047 after deleting the initial 0.
 * https://oeis.org/A258049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (index k of the n-th positive term in A258047's
 *            d-sequence) - 1; computed by direct simulation of Rule 3
 *            (a(1)=1, d(1)=0) and listing positions of positive d
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 949 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static char *s_A = NULL, *s_Dp = NULL, *s_Dn = NULL;
static size_t cA = 0, cDp = 0, cDn = 0;
static long long *pos_list = NULL;
static size_t pos_cap = 0, pos_cnt = 0;
static long long cur_a = 1;
static long long cur_len = 0;
static bool inited = false;

static void grow_set(char **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t nc = *cap ? *cap : 256;
    while (nc < need) nc <<= 1;
    char *p = (char *)realloc(*arr, nc);
    if (!p) exit(1);
    memset(p + *cap, 0, nc - *cap);
    *arr = p;
    *cap = nc;
}

static void add_pos(long long idx)
{
    if (pos_cnt >= pos_cap) {
        size_t nc = pos_cap ? pos_cap * 2 : 256;
        long long *p = (long long *)realloc(pos_list, nc * sizeof(long long));
        if (!p) exit(1);
        pos_list = p;
        pos_cap = nc;
    }
    pos_list[pos_cnt++] = idx;
}

static void record(long long newa, long long h)
{
    grow_set(&s_A, &cA, (size_t)newa + 2);
    s_A[newa] = 1;
    if (h > 0) {
        grow_set(&s_Dp, &cDp, (size_t)h + 2);
        s_Dp[h] = 1;
    } else {
        grow_set(&s_Dn, &cDn, (size_t)(-h) + 2);
        s_Dn[-h] = 1;
    }
    cur_a = newa;
    cur_len++;
    if (h > 0) add_pos(cur_len);
}

static void step(void)
{
    long long ak = cur_a;
    grow_set(&s_Dn, &cDn, (size_t)ak + 2);
    grow_set(&s_A, &cA, (size_t)ak + 2);
    for (long long h = 2 - ak; h <= -1; ++h) {
        if (!s_Dn[-h] && !s_A[ak + h]) {
            record(ak + h, h);
            return;
        }
    }
    for (long long h = 1;; ++h) {
        grow_set(&s_Dp, &cDp, (size_t)h + 2);
        long long b = ak - h;
        bool inA = (b >= 1) && s_A[b];
        if (!s_Dp[h] && !inA) {
            record(ak + h, h);
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!inited) {
        inited = true;
        grow_set(&s_A, &cA, 256);
        grow_set(&s_Dp, &cDp, 256);
        grow_set(&s_Dn, &cDn, 256);
        s_A[1] = 1;
        cur_a = 1;
        cur_len = 1;   /* d(1) = 0 */
    }
    while (pos_cnt < (size_t)n) step();
    return (seq_t)(pos_list[n - 1] - 1);
}
