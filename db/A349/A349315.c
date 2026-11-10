/*
 * A349315  Ordinal transform of A348674.
 * https://oeis.org/A349315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = #{ m in 0..n : A348674(m) = A348674(n) }, computed by
 *            brute force (A348674 via enumerating all digit splits)
 * Range:     u8 n <= 424, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 86 terms through n = 9685 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A349315_cmp(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static uint64_t A349315_f(uint64_t n)
{
    int d[20];
    int len = 0;
    if (n == 0) { d[0] = 0; len = 1; }
    else {
        uint64_t t = n;
        int tmp[20];
        while (t > 0) { tmp[len++] = (int)(t % 10); t /= 10; }
        for (int i = 0; i < len; ++i) d[i] = tmp[len - 1 - i];
    }
    size_t splits = (size_t)1 << (len - 1);
    uint64_t *sums = (uint64_t *)malloc(splits * sizeof(uint64_t));
    if (!sums) return 0;
    for (size_t mask = 0; mask < splits; ++mask) {
        uint64_t cur = 0, sum = 0;
        for (int i = 0; i < len; ++i) {
            cur = cur * 10 + (uint64_t)d[i];
            if (i == len - 1 || (mask & ((size_t)1 << i))) { sum += cur; cur = 0; }
        }
        sums[mask] = sum;
    }
    if (splits > 1) qsort(sums, splits, sizeof(uint64_t), A349315_cmp);
    uint64_t uniq = 1;
    for (size_t i = 1; i < splits; ++i)
        if (sums[i] != sums[i - 1]) ++uniq;
    free(sums);
    return uniq;
}

seq_t a(idx_t n)
{
    uint64_t target = A349315_f((uint64_t)n);
    uint64_t cnt = 0;
    for (uint64_t m = 0; m <= (uint64_t)n; ++m)
        if (A349315_f(m) == target) ++cnt;
    return (seq_t)cnt;
}
