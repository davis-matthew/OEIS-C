/*
 * A373490  Numbers k for which A003415(k) is a multiple of A276085(k), where
 *          A003415 is the arithmetic derivative and A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A373490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A276085(k) | A003415(k); k=1 is included
 *            by convention. Computed by DFS over smooth numbers (only primes
 *            p with (p#/p) <= k*log2(k)*... can occur), with the limit grown
 *            geometrically and the results sorted
 * Range:     u8 n <= 23, u16 n <= 112, u32 n <= 1446, u64 n <= 1710,
 *            u128 n <= 1710, big any n
 * Validated: standard, pass, 77 terms through n = 1664 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 u128_t;

static const unsigned SP_primes[18] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};
static u128_t SP_prev[18];
static u128_t *SP_res = NULL;
static size_t SP_cnt = 0, SP_cap = 0;
static u128_t SP_L = 0;
static u128_t SP_capd = 0;
static bool SP_init = false;
static bool SP_done = false;

static void SP_push(u128_t v) {
    if (SP_cnt == SP_cap) {
        SP_cap = SP_cap ? SP_cap * 2 : 256;
        SP_res = (u128_t *)realloc(SP_res, SP_cap * sizeof(u128_t));
    }
    SP_res[SP_cnt++] = v;
}

static void SP_dfs(int i, u128_t n, u128_t d, u128_t s) {
    for (int j = i; j < 18; j++) {
        if (SP_prev[j] > SP_capd) break;
        u128_t p = SP_primes[j];
        if (n > SP_L / p) break;
        u128_t m = n * p;
        u128_t dd = d * p + n;
        u128_t ss = s + SP_prev[j];
        for (;;) {
            if (ss > 0 && dd % ss == 0) SP_push(m);
            SP_dfs(j + 1, m, dd, ss);
            if (m > SP_L / p) break;
            dd = dd * p + m;
            m = m * p;
            ss += SP_prev[j];
        }
    }
}

static int SP_cmp(const void *a, const void *b) {
    u128_t x = *(const u128_t *)a, y = *(const u128_t *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

seq_t a(idx_t n) {
    if (!SP_init) {
        u128_t prod = 1;
        for (int i = 0; i < 18; i++) {
            SP_prev[i] = prod;
            prod *= SP_primes[i];
        }
        SP_init = true;
    }
    if (n == 1) return (seq_t)1;
    const u128_t MAXL = ((u128_t)1) << 64;
    while (!SP_done && (SP_L == 0 || (u128_t)SP_cnt + 1 < (u128_t)n)) {
        if (SP_L == 0) SP_L = 1024;
        else SP_L *= 8;
        if (SP_L >= MAXL) { SP_L = MAXL; SP_done = true; }
        SP_capd = SP_L * 64;
        SP_cnt = 0;
        SP_dfs(0, 1, 0, 0);
        qsort(SP_res, SP_cnt, sizeof(u128_t), SP_cmp);
    }
    /* SP_res holds terms > 1; a(n) = SP_res[n-2] */
    if ((size_t)(n - 2) < SP_cnt) return (seq_t)SP_res[n - 2];
    return (seq_t)0;
}
