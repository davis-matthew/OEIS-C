/*
 * A324292  Lexicographically earliest sequence such that a(i) = a(j) =>
 *          A324291(i) = A324291(j) for all i, j >= 1.
 * https://oeis.org/A324292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of A324291, where A324291(n)
 *            = Product_{p|n prime} prime(1+((n-1) mod (p-1))); a(n) = rank of
 *            A324291(n) among distinct values in order of first appearance
 * Range:     u8 n <= 2448, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9705 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned __int128 a324292_key_t;

struct a324292_item {
    a324292_key_t key;
    uint32_t idx;
};

static int a324292_cmp(const void *x, const void *y)
{
    const struct a324292_item *a = (const struct a324292_item *)x;
    const struct a324292_item *b = (const struct a324292_item *)y;
    if (a->key < b->key) return -1;
    if (a->key > b->key) return 1;
    if (a->idx < b->idx) return -1;
    if (a->idx > b->idx) return 1;
    return 0;
}

static uint32_t *a324292_cache = NULL;
static uint64_t a324292_cache_n = 0;

static void a324292_build(uint64_t N)
{
    uint64_t lim = N * 20 + 100;
    uint32_t *spf = (uint32_t *)calloc((size_t)(N + 2), sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)(lim + 1), 1);
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(N + 2));
    size_t np = 0;
    uint64_t i, j;
    for (i = 2; i <= lim && np < N + 1; i++) {
        if (!comp[i]) {
            pr[np++] = (uint32_t)i;
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    struct a324292_item *it = (struct a324292_item *)malloc(sizeof(struct a324292_item) * (size_t)N);
    for (i = 1; i <= N; i++) {
        a324292_key_t key = 1;
        uint64_t m = i;
        while (m > 1) {
            uint64_t p = spf[m];
            while (m % p == 0) m /= p;
            uint64_t k = ((i - 1) % (p - 1)) + 1;
            key *= (a324292_key_t)pr[k - 1];
        }
        it[i - 1].key = key;
        it[i - 1].idx = (uint32_t)i;
    }
    qsort(it, (size_t)N, sizeof(struct a324292_item), a324292_cmp);
    uint32_t *first = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(N + 1));
    for (i = 0; i < N; i++) {
        if (i > 0 && it[i].key == it[i - 1].key)
            first[it[i].idx] = first[it[i - 1].idx];
        else
            first[it[i].idx] = it[i].idx;
    }
    free(a324292_cache);
    a324292_cache = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(N + 1));
    uint32_t c = 0;
    a324292_cache[0] = 0;
    for (i = 1; i <= N; i++) {
        if (first[i] == i) a324292_cache[i] = ++c;
        else a324292_cache[i] = a324292_cache[first[i]];
    }
    a324292_cache_n = N;
    free(first);
    free(it);
    free(pr);
    free(comp);
    free(spf);
}

seq_t a(idx_t n)
{
    if (n > a324292_cache_n) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * a324292_cache_n) N = 2 * a324292_cache_n;
        if (N < 1024) N = 1024;
        a324292_build(N);
    }
    return (seq_t)a324292_cache[n];
}
