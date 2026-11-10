/*
 * A331521  a(n) is the least positive k such that A002645(n) - k^4 is a
 *          fourth power.
 * https://oeis.org/A331521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(x,y) where A002645(n) = x^4 + y^4 (the representation
 *            is unique); computed by search + sort
 * Range:     u8 n <= 4656, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 8791 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

typedef struct { uint64_t p; uint64_t k; } A331521_pair;

static int A331521_cmp(const void *a, const void *b) {
    uint64_t u = ((const A331521_pair *)a)->p, v = ((const A331521_pair *)b)->p;
    return (u > v) - (u < v);
}

static A331521_pair *A331521_cache = NULL;
static size_t A331521_len = 0;
static uint64_t A331521_limit = 0;

static void A331521_build(uint64_t limit) {
    size_t cap = 1024, len = 0;
    A331521_pair *v = (A331521_pair *)malloc(cap * sizeof(A331521_pair));
    uint64_t x, y;
    for (x = 1; x * x * x * x < limit; x++) {
        uint64_t x4 = x * x * x * x;
        for (y = x; ; y++) {
            uint64_t s = x4 + y * y * y * y;
            if (s > limit) break;
            if (((x + y) & 1) == 0 && !(x == 1 && y == 1)) continue;
            if (is_prime_u64(s)) {
                if (len == cap) {
                    cap *= 2;
                    v = (A331521_pair *)realloc(v, cap * sizeof(A331521_pair));
                }
                v[len].p = s;
                v[len].k = x;
                len++;
            }
        }
    }
    qsort(v, len, sizeof(A331521_pair), A331521_cmp);
    free(A331521_cache);
    A331521_cache = v;
    A331521_len = len;
    A331521_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t limit = A331521_limit ? A331521_limit : ((uint64_t)1 << 20);
    if (A331521_limit == 0) A331521_build(limit);
    while ((idx_t)A331521_len < n) {
        limit *= 4;
        A331521_build(limit);
    }
    return (seq_t)A331521_cache[n - 1].k;
}
