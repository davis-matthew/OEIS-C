/*
 * A269367  Suspected permutation of natural numbers: a(n) =
 *          A255070(A269363(n)).
 * https://oeis.org/A269367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n+1) = least unused m such that
 *            A091067(a(n))*A091067(m) has no two adjacent 1's in binary
 *            (greedy search; A003714 membership test)
 * Range:     u8 n <= 160, u16 n <= 13217, u32 n <= 13217, u64 n <= 13217,
 *            u128 n <= 13217, big any n
 * Validated: standard, pass, 101 terms through n = 9883 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A269367_v = NULL;   /* A091067 values, 1-indexed */
static unsigned char *A269367_used = NULL;
static uint64_t A269367_cap = 0, A269367_vn = 0, A269367_c = 2;
static uint64_t *A269367_res = NULL;
static uint64_t A269367_rcap = 0, A269367_rn = 0, A269367_low = 2;

static void A269367_ensure(uint64_t m)
{
    if (m >= A269367_cap) {
        uint64_t nc = A269367_cap ? A269367_cap : 1024;
        while (nc <= m) nc *= 2;
        A269367_v = (uint64_t *)realloc(A269367_v, nc * sizeof(uint64_t));
        A269367_used = (unsigned char *)realloc(A269367_used, nc);
        memset(A269367_used + A269367_cap, 0, nc - A269367_cap);
        A269367_cap = nc;
    }
    while (A269367_vn < m) {
        uint64_t x = A269367_c++;
        while ((x & 1u) == 0) x >>= 1;
        if ((x & 3u) == 3u) {
            A269367_vn++;
            A269367_v[A269367_vn] = A269367_c - 1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A269367_rn == 0) {
        A269367_rcap = 1024;
        A269367_res = (uint64_t *)malloc(A269367_rcap * sizeof(uint64_t));
        A269367_ensure(2);
        A269367_res[1] = 1;
        A269367_used[1] = 1;
        A269367_rn = 1;
        A269367_low = 2;
    }
    while (A269367_rn < (uint64_t)n) {
        if (A269367_rn + 1 >= A269367_rcap) {
            A269367_rcap *= 2;
            A269367_res = (uint64_t *)realloc(A269367_res, A269367_rcap * sizeof(uint64_t));
        }
        uint64_t cur = A269367_res[A269367_rn];
        A269367_ensure(cur);
        uint64_t vc = A269367_v[cur];
        uint64_t m = A269367_low;
        for (;; m++) {
            A269367_ensure(m);
            if (A269367_used[m]) continue;
            uint64_t p = vc * A269367_v[m];
            if ((p & (p >> 1)) == 0) break;
        }
        A269367_used[m] = 1;
        A269367_res[++A269367_rn] = m;
        while (1) {
            A269367_ensure(A269367_low);
            if (!A269367_used[A269367_low]) break;
            A269367_low++;
        }
    }
    return (seq_t)A269367_res[n];
}
