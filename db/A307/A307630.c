/*
 * A307630  Index at which n first appears in A307720.
 * https://oeis.org/A307630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- simulate A307720 greedily (a(1)=1; next term = least x
 *            with fewer than prev*x earlier pairs of product prev*x), and
 *            record the index where each value first appears. Runs that
 *            alternate u,v,u,v,... on the same product u*v are skipped over
 *            in one step
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 521, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9742 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A307630_cnt = NULL;   /* cnt[k] = pairs with product k */
static size_t A307630_cnt_cap = 0;
static uint64_t *A307630_ptr = NULL;   /* ptr[v] = lower bound for the next x after v */
static size_t A307630_ptr_cap = 0;
static uint64_t *A307630_first = NULL; /* first[v] = index where v first appears, 0 if not yet */
static size_t A307630_first_cap = 0;
static uint64_t A307630_prev = 0;      /* last term */
static uint64_t A307630_pprev = 0;     /* term before last (0 = none) */
static uint64_t A307630_len = 0;       /* number of terms so far */

static void A307630_ensure32(uint32_t **arr, size_t *cap, uint64_t idx)
{
    if ((size_t)idx < *cap) return;
    size_t nc = *cap ? *cap : 1024;
    while (nc <= (size_t)idx) nc <<= 1;
    uint32_t *t = (uint32_t *)realloc(*arr, nc * sizeof(uint32_t));
    memset(t + *cap, 0, (nc - *cap) * sizeof(uint32_t));
    *arr = t;
    *cap = nc;
}

static void A307630_ensure64(uint64_t **arr, size_t *cap, uint64_t idx)
{
    if ((size_t)idx < *cap) return;
    size_t nc = *cap ? *cap : 1024;
    while (nc <= (size_t)idx) nc <<= 1;
    uint64_t *t = (uint64_t *)realloc(*arr, nc * sizeof(uint64_t));
    memset(t + *cap, 0, (nc - *cap) * sizeof(uint64_t));
    *arr = t;
    *cap = nc;
}

seq_t a(idx_t n)
{
    if (A307630_len == 0) {
        A307630_ensure32(&A307630_cnt, &A307630_cnt_cap, 1024);
        A307630_ensure64(&A307630_ptr, &A307630_ptr_cap, 1024);
        A307630_ensure64(&A307630_first, &A307630_first_cap, 1024);
        A307630_prev = 1;
        A307630_pprev = 0;
        A307630_len = 1;
        A307630_first[1] = 1;
    }
    uint64_t target = (uint64_t)n;
    for (;;) {
        if ((size_t)target < A307630_first_cap && A307630_first[target] != 0)
            return (seq_t)A307630_first[target];

        uint64_t pv = A307630_prev;
        uint64_t pp = A307630_pprev;
        A307630_ensure64(&A307630_ptr, &A307630_ptr_cap, pv);
        uint64_t x = A307630_ptr[pv];
        if (x == 0) x = 1;
        uint64_t p;
        for (;;) {
            p = pv * x;
            A307630_ensure32(&A307630_cnt, &A307630_cnt_cap, p);
            if ((uint64_t)A307630_cnt[p] < p) break;
            x++;
        }
        A307630_ptr[pv] = x;

        if (x == pp) {
            /* alternation pp,pv,pp,pv,... continues until product p is full */
            uint64_t r = p - (uint64_t)A307630_cnt[p];
            A307630_cnt[p] = (uint32_t)p;
            A307630_len += r;
            A307630_ensure64(&A307630_ptr, &A307630_ptr_cap, pp);
            A307630_ptr[pp] = pv;
            if (r & 1) {
                A307630_prev = pp;
                A307630_pprev = pv;
            } else {
                A307630_prev = pv;
                A307630_pprev = pp;
            }
            continue;
        }

        A307630_cnt[p]++;
        A307630_len++;
        A307630_pprev = pv;
        A307630_prev = x;
        A307630_ensure64(&A307630_first, &A307630_first_cap, x);
        if (A307630_first[x] == 0) A307630_first[x] = A307630_len;
    }
}
