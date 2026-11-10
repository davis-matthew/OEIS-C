/*
 * A388044  a(n) = number of k <= m such that rad(k) | m, where m = A001694(n)
 *          and rad(n) = A007947(n).
 * https://oeis.org/A388044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010846(A001694(n)); m = n-th powerful number found by
 *            binary search on the count of powerful numbers <= x, then count
 *            k <= m with all prime factors dividing m by DFS over the primes
 *            of m
 * Range:     u8 n <= 399, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9775 (2026-11-10)
 * Helpers:   is_squarefree_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree_u64.h"
#include "../utils/isqrt_u64.h"

static uint64_t A388044_pc(uint64_t x) {
    uint64_t total = 0, b;
    for (b = 1; b * b * b <= x; b++)
        if (is_squarefree_u64(b))
            total += isqrt_u64(x / (b * b * b));
    return total;
}

static uint64_t A388044_pow(uint64_t n) {
    uint64_t lo = 1, hi = n * n;
    if (hi < 1) hi = 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A388044_pc(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

static uint64_t A388044_dfs(const uint64_t *pr, int cnt, int i, uint64_t lim) {
    uint64_t total = 0, v;
    if (i == cnt) return 1;
    v = lim;
    while (v > 0) {
        total += A388044_dfs(pr, cnt, i + 1, v);
        v /= pr[i];
    }
    return total;
}

seq_t a(idx_t n) {
    uint64_t m = A388044_pow((uint64_t)n);
    uint64_t pr[24];
    int cnt = 0;
    uint64_t t = m, p;
    for (p = 2; p <= t / p; p++) {
        if (t % p == 0) {
            pr[cnt++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) pr[cnt++] = t;
    return (seq_t)A388044_dfs(pr, cnt, 0, m);
}
