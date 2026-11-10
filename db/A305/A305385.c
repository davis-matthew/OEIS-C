/*
 * A305385  Indicator function of A140100.
 * https://oeis.org/A305385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is in A140100 (the X(n) values of the greedy
 *            complementary X/Y construction), else 0; computed by simulation
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A305385_X = NULL;
static size_t A305385_cnt = 0, A305385_cap = 0;
static unsigned char *A305385_used = NULL, *A305385_diff = NULL, *A305385_sum = NULL;
static size_t A305385_ucap = 0, A305385_dcap = 0, A305385_scap = 0;
static uint64_t A305385_nx = 1;

static void A305385_ens(unsigned char **arr, size_t *cap, size_t need) {
    if (need < *cap) return;
    size_t nc = (*cap == 0) ? 256 : *cap;
    while (nc <= need) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(*arr, nc);
    if (!t) abort();
    memset(t + *cap, 0, nc - *cap);
    *arr = t;
    *cap = nc;
}

static void A305385_add(uint64_t x) {
    if (A305385_cnt == A305385_cap) {
        size_t nc = A305385_cap ? A305385_cap * 2 : 256;
        uint64_t *t = (uint64_t *)realloc(A305385_X, nc * sizeof(uint64_t));
        if (!t) abort();
        A305385_X = t;
        A305385_cap = nc;
    }
    A305385_X[A305385_cnt++] = x;
}

static void A305385_step(void) {
    if (A305385_cnt == 0) {
        A305385_ens(&A305385_used, &A305385_ucap, 2);
        A305385_ens(&A305385_diff, &A305385_dcap, 1);
        A305385_ens(&A305385_sum, &A305385_scap, 3);
        A305385_used[1] = A305385_used[2] = 1;
        A305385_diff[1] = 1;
        A305385_sum[3] = 1;
        A305385_add(1);
        return;
    }
    uint64_t x = A305385_nx;
    for (;;) {
        A305385_ens(&A305385_used, &A305385_ucap, (size_t)x);
        if (!A305385_used[x]) break;
        ++x;
    }
    A305385_nx = x;
    uint64_t y = x + 1;
    for (;;) {
        A305385_ens(&A305385_used, &A305385_ucap, (size_t)y);
        if (A305385_used[y]) { ++y; continue; }
        uint64_t d = y - x, s = y + x;
        A305385_ens(&A305385_diff, &A305385_dcap, (size_t)d);
        A305385_ens(&A305385_sum, &A305385_scap, (size_t)s);
        if (A305385_diff[d] || A305385_sum[d]) { ++y; continue; }
        A305385_used[x] = A305385_used[y] = 1;
        A305385_diff[d] = 1;
        A305385_sum[s] = 1;
        A305385_add(x);
        break;
    }
}

seq_t a(idx_t n) {
    uint64_t t = (uint64_t)n;
    while (A305385_cnt == 0 || A305385_X[A305385_cnt - 1] < t) A305385_step();
    size_t lo = 0, hi = A305385_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A305385_X[mid] < t) lo = mid + 1; else hi = mid;
    }
    return (seq_t)((lo < A305385_cnt && A305385_X[lo] == t) ? 1 : 0);
}
