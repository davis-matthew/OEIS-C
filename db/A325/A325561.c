/*
 * A325561  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j) where f(n) = A234741(n) except for those
 *          numbers for which A234741(n) = n, in which case f(n) = 0.
 * https://oeis.org/A325561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth numbering (order of first appearance)
 *            of f(n), where f(n) = 0 if A234741(n) = n, else A234741(n);
 *            computed by a sieve-like scan with a hash table
 * Range:     u8 n <= 722, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8933 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A234741_clmul(uint64_t x, uint64_t y)
{
    uint64_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static uint64_t A234741_val(uint64_t n)
{
    uint64_t result = 1, m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        while (m % p == 0) {
            result = A234741_clmul(result, p);
            m /= p;
        }
    }
    if (m > 1) result = A234741_clmul(result, m);
    return result;
}

static uint32_t *cls_cache = NULL;
static uint64_t cls_n = 0;

static void build_classes(uint64_t N)
{
    free(cls_cache);
    cls_cache = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint64_t sz = 1;
    while (sz < 2 * N + 2) sz <<= 1;
    uint64_t *keys = (uint64_t *)malloc(sz * sizeof(uint64_t));
    uint32_t *vals = (uint32_t *)calloc(sz, sizeof(uint32_t)); /* 0 = empty */
    uint32_t next = 1;
    for (uint64_t m = 1; m <= N; ++m) {
        uint64_t v = A234741_val(m);
        if (v == m) v = 0;
        uint64_t h = (v * 0x9E3779B97F4A7C15ULL) >> 20;
        h &= (sz - 1);
        while (vals[h] != 0 && keys[h] != v) h = (h + 1) & (sz - 1);
        if (vals[h] == 0) {
            keys[h] = v;
            vals[h] = next++;
        }
        cls_cache[m] = vals[h];
    }
    free(keys);
    free(vals);
    cls_n = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((uint64_t)n > cls_n) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * cls_n) N = 2 * cls_n;
        if (N < 1024) N = 1024;
        build_classes(N);
    }
    return (seq_t)cls_cache[n];
}
