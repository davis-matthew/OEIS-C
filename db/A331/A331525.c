/*
 * A331525  a(n) is the least positive k such that A100272(n) - k^2 is a fifth
 *          power.
 * https://oeis.org/A331525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- list the primes p = x^2 + y^5 with x, y >= 1 (the
 *            terms of A100272) in increasing order, and for each p take the
 *            least such x
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8843 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

typedef struct { uint64_t m, k; } pr_t;

static int cmp_pr(const void *a, const void *b) {
    const pr_t *x = (const pr_t *)a, *y = (const pr_t *)b;
    if (x->m != y->m) return x->m < y->m ? -1 : 1;
    if (x->k != y->k) return x->k < y->k ? -1 : 1;
    return 0;
}

static uint64_t *cache_k = 0;
static uint64_t cache_cnt = 0;
static uint64_t cache_L = 0;

static void build(uint64_t L) {
    size_t cap = 1024, cnt = 0;
    pr_t *v = (pr_t *)malloc(cap * sizeof(pr_t));
    for (uint64_t y = 1; y * y * y * y * y < L; y++) {
        uint64_t y5 = y * y * y * y * y;
        for (uint64_t x = 1; x * x + y5 <= L; x++) {
            uint64_t m = x * x + y5;
            if (is_prime_u64(m)) {
                if (cnt == cap) {
                    cap *= 2;
                    v = (pr_t *)realloc(v, cap * sizeof(pr_t));
                }
                v[cnt].m = m;
                v[cnt].k = x;
                cnt++;
            }
        }
    }
    qsort(v, cnt, sizeof(pr_t), cmp_pr);
    free(cache_k);
    cache_k = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    uint64_t c = 0;
    for (size_t i = 0; i < cnt; i++) {
        if (i == 0 || v[i].m != v[i - 1].m) cache_k[c++] = v[i].k;
    }
    cache_cnt = c;
    cache_L = L;
    free(v);
}

seq_t a(idx_t n) {
    if (cache_L == 0) build((uint64_t)1 << 12);
    while (cache_cnt < (uint64_t)n) build(cache_L * 4);
    return (seq_t)cache_k[n - 1];
}
