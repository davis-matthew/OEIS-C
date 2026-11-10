/*
 * A358477  Number of 4's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4's among the terms appended to the list in
 *            step n-1 of the A030717 construction (a(1) = 0). The code
 *            simulates the construction
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static size_t *A358477_cnt = NULL;   /* frequency of each value in L1 */
static size_t  A358477_cap = 0;
static size_t  A358477_len = 0;      /* length of L1 */
static size_t *A358477_res = NULL;   /* res[k] = 4's in step k (1-based) */
static size_t  A358477_steps = 0;
static size_t  A358477_rcap = 0;

static void A358477_step(void)
{
    size_t need = A358477_len + 4;
    if (need > A358477_cap) {
        size_t nc = A358477_cap ? A358477_cap : 16;
        while (nc < need * 2) nc <<= 1;
        size_t *p = (size_t *)realloc(A358477_cnt, nc * sizeof(size_t));
        for (size_t i = A358477_cap; i < nc; ++i) p[i] = 0;
        A358477_cnt = p;
        A358477_cap = nc;
    }
    size_t *tmp = (size_t *)malloc((A358477_len + 2) * sizeof(size_t));
    size_t m = 0, fours = 0;
    for (size_t v = 1; v <= A358477_len; ++v) {
        if (A358477_cnt[v]) {
            tmp[m++] = A358477_cnt[v];
            if (A358477_cnt[v] == 4) fours++;
        }
    }
    for (size_t i = 0; i < m; ++i) A358477_cnt[tmp[i]]++;
    A358477_len += m;
    free(tmp);
    if (A358477_steps + 2 > A358477_rcap) {
        A358477_rcap = A358477_rcap ? A358477_rcap * 2 : 64;
        A358477_res = (size_t *)realloc(A358477_res, A358477_rcap * sizeof(size_t));
    }
    A358477_steps++;
    A358477_res[A358477_steps] = fours;
}

seq_t a(idx_t n)
{
    if (A358477_cnt == NULL) {
        A358477_cap = 64;
        A358477_cnt = (size_t *)calloc(A358477_cap, sizeof(size_t));
        A358477_cnt[1] = 1;
        A358477_len = 1;
    }
    if (n <= 1) return (seq_t)0;
    while ((idx_t)A358477_steps < n - 1) A358477_step();
    return (seq_t)A358477_res[n - 1];
}
