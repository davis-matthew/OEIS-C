/*
 * A342951  a(n) is the least term in A007602 that is divisible by A342950(n).
 * https://oeis.org/A342951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least term of A007602 divisible by A342950(n); computed
 *            by search over A007602 terms (up to 19 digits), in increasing
 *            order
 * Range:     u8 n <= 14, u16 n <= 39, u32 n <= 140, u64 n <= 335,
 *            u128 n <= 335, big any n
 * Validated: standard, pass, 71 terms through n = 297 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ---- A007602 (renamed copy) ---- */
static int z_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void z_push(uint64_t **arr, size_t *n, size_t *cap, uint64_t v) {
    if (*n == *cap) {
        size_t nc = *cap ? *cap * 2 : 1024;
        *arr = (uint64_t *)realloc(*arr, nc * sizeof(uint64_t));
        *cap = nc;
    }
    (*arr)[(*n)++] = v;
}

static uint64_t *z_terms = NULL;
static size_t z_nterms = 0, z_cap = 0;
static uint64_t *z_buf = NULL;
static size_t z_nbuf = 0, z_bcap = 0;
static int z_len = 0;
static int z_cnt[10];
static int z_L, z_A, z_C;
static uint64_t z_P;
static uint64_t z_pow5[21];

static void z_dfs(int j, uint64_t suf, uint64_t p10) {
    int d;
    if (j == z_L) {
        if (suf % z_P == 0) z_push(&z_buf, &z_nbuf, &z_bcap, suf);
        return;
    }
    for (d = 1; d <= 9; d++) {
        uint64_t s;
        int k;
        if (!z_cnt[d]) continue;
        s = suf + (uint64_t)d * p10;
        k = j + 1;
        if (z_A) {
            int e = k < z_A ? k : z_A;
            if (s & ((1ULL << e) - 1)) continue;
        }
        if (z_C) {
            int e = k < z_C ? k : z_C;
            if (s % z_pow5[e]) continue;
        }
        z_cnt[d]--;
        z_dfs(k, s, p10 * 10);
        z_cnt[d]++;
    }
}

static void z_process(void) {
    int d, i, b, sum;
    bool has5 = z_cnt[5] > 0;
    bool haseven = (z_cnt[2] + z_cnt[4] + z_cnt[6] + z_cnt[8]) > 0;
    if (has5 && haseven) return;
    z_P = 1;
    sum = 0;
    for (d = 1; d <= 9; d++) {
        for (i = 0; i < z_cnt[d]; i++) z_P *= (uint64_t)d;
        sum += d * z_cnt[d];
    }
    z_A = z_cnt[2] + 2 * z_cnt[4] + z_cnt[6] + 3 * z_cnt[8];
    z_C = z_cnt[5];
    b = z_cnt[3] + z_cnt[6] + 2 * z_cnt[9];
    if (b >= 1 && sum % 3 != 0) return;
    if (b >= 2 && sum % 9 != 0) return;
    z_dfs(0, 0, 1);
}

static void z_ms(int d, int rem) {
    int c;
    if (d == 9) {
        z_cnt[9] = rem;
        z_process();
        return;
    }
    for (c = 0; c <= rem; c++) {
        z_cnt[d] = c;
        z_ms(d + 1, rem - c);
    }
    z_cnt[d] = 0;
}

static void z_gen_next(void) {
    size_t i;
    int d;
    z_L = z_len + 1;
    for (d = 0; d < 10; d++) z_cnt[d] = 0;
    z_nbuf = 0;
    z_ms(1, z_L);
    qsort(z_buf, z_nbuf, sizeof(uint64_t), z_cmp);
    for (i = 0; i < z_nbuf; i++) z_push(&z_terms, &z_nterms, &z_cap, z_buf[i]);
    z_len = z_L;
}

static uint64_t A007602_a(uint64_t n) {
    if (z_pow5[0] == 0) {
        int i;
        z_pow5[0] = 1;
        for (i = 1; i < 21; i++) z_pow5[i] = z_pow5[i - 1] * 5;
    }
    while ((uint64_t)z_nterms < n && z_len < 19) z_gen_next();
    if (n == 0 || (uint64_t)z_nterms < n) return 0;
    return z_terms[n - 1];
}

/* ---- A342950 (renamed copy) ---- */
static void A342950_ensure(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t ncap = *cap ? *cap : 4;
    while (ncap < needed) ncap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, ncap);
    *arr = tmp;
    *cap = ncap;
}

static seq_t A342950_a(idx_t n)
{
    if (n == 0)
        return 0;
    static seq_t *smooth = NULL;
    static size_t smooth_cap = 0, smooth_len = 0;
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;
    static seq_t *valid = NULL;
    static size_t valid_cap = 0, valid_len = 0;

    if (smooth_len == 0) {
        A342950_ensure(&smooth, &smooth_cap, 4);
        smooth[0] = (seq_t)1;
        smooth_len = 1;
        i2 = i3 = i5 = i7 = 0;
        A342950_ensure(&valid, &valid_cap, 4);
        valid[0] = (seq_t)1;
        valid_len = 1;
    }
    if (valid_len >= n)
        return valid[n - 1];
    while (valid_len < n) {
        seq_t two   = smooth[i2] * 2;
        seq_t three = smooth[i3] * 3;
        seq_t five  = smooth[i5] * 5;
        seq_t seven = smooth[i7] * 7;
        seq_t next = two;
        if (three < next) next = three;
        if (five  < next) next = five;
        if (seven < next) next = seven;
        A342950_ensure(&smooth, &smooth_cap, smooth_len + 1);
        smooth[smooth_len++] = next;
        if (next == two)   ++i2;
        if (next == three) ++i3;
        if (next == five)  ++i5;
        if (next == seven) ++i7;
        if (next % 10 != 0) {
            A342950_ensure(&valid, &valid_cap, valid_len + 1);
            valid[valid_len++] = next;
            if (valid_len == n)
                return next;
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t m = A342950_a(n);
    uint64_t k;
    if (n == 0 || m == 0) return (seq_t)0;
    for (k = 1;; k++) {
        uint64_t v = A007602_a(k);
        if (v == 0) break;
        if (((seq_t)v) % m == 0) return (seq_t)v;
    }
    return (seq_t)0;
}
