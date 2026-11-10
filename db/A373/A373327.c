/*
 * A373327  Records of A064784.
 * https://oeis.org/A373327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th record value of A064784(m) = m*(m+1)/2 -
 *            floor(sqrt(m*(m+1)/2))^2, m >= 1. Scans m with native 64-bit
 *            arithmetic and memoizes every record found
 * Range:     u8 n <= 28, u16 n <= 466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9952 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *A373327_rec = NULL;   /* record values, native */
static size_t A373327_cnt = 0;
static size_t A373327_cap = 0;
/* scan state */
static uint64_t A373327_m = 0;     /* last m processed */
static uint64_t A373327_t = 0;     /* t(m) */
static uint64_t A373327_r = 0;     /* floor(sqrt(t)) */
static uint64_t A373327_r2 = 0;    /* r*r */
static uint64_t A373327_nx = 1;    /* (r+1)^2 */
static uint64_t A373327_best = 0;
static bool A373327_have = false;

static void A373327_push(uint64_t v)
{
    if (A373327_cnt == A373327_cap) {
        size_t nc = A373327_cap ? A373327_cap * 2 : 1024;
        uint64_t *p = (uint64_t *)realloc(A373327_rec, nc * sizeof(uint64_t));
        if (!p) return;
        A373327_rec = p;
        A373327_cap = nc;
    }
    A373327_rec[A373327_cnt++] = v;
}

seq_t a(idx_t n)
{
    while ((idx_t)A373327_cnt < n) {
        A373327_m++;
        A373327_t += A373327_m;
        while (A373327_nx <= A373327_t) {
            A373327_r++;
            A373327_r2 = A373327_nx;
            A373327_nx = (A373327_r + 1) * (A373327_r + 1);
        }
        uint64_t d = A373327_t - A373327_r2;
        if (!A373327_have || d > A373327_best) {
            A373327_have = true;
            A373327_best = d;
            size_t before = A373327_cnt;
            A373327_push(d);
            if (A373327_cnt == before) return (seq_t)0; /* allocation failure */
        }
    }
    return (seq_t)A373327_rec[n - 1];
}
