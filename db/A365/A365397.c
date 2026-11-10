/*
 * A365397  a(n) = 64 + A000720(n) - A365398(n).
 * https://oeis.org/A365397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64 + A000720(n) - A365398(n); A365398(n) = length of
 *            longest nondecreasing subsequence of sigma(1..n), computed by a
 *            sigma sieve plus O(n log n) LNDS; pi(n) counts i with
 *            sigma(i)=i+1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *a365397_cache = NULL;
static idx_t a365397_N = 0;

static void a365397_build(idx_t N) {
    uint64_t *sg = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    uint64_t *tails = (uint64_t *)malloc(((size_t)N + 1) * sizeof(uint64_t));
    uint32_t *res = (uint32_t *)malloc(((size_t)N + 1) * sizeof(uint32_t));
    if (!sg || !tails || !res) { free(sg); free(tails); free(res); return; }
    for (size_t d = 1; d <= (size_t)N; d++)
        for (size_t m = d; m <= (size_t)N; m += d) sg[m] += d;
    size_t len = 0;
    uint64_t pi = 0;
    res[0] = 0;
    for (size_t i = 1; i <= (size_t)N; i++) {
        uint64_t x = sg[i];
        if (i > 1 && x == i + 1) pi++;
        size_t lo = 0, hi = len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (tails[mid] > x) hi = mid; else lo = mid + 1;
        }
        if (lo == len) tails[len++] = x; else tails[lo] = x;
        res[i] = (uint32_t)(64 + pi - len);
    }
    free(sg); free(tails);
    free(a365397_cache);
    a365397_cache = res;
    a365397_N = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a365397_N) {
        idx_t N = n;
        if (N < 2 * a365397_N) N = 2 * a365397_N;
        if (N < 1024) N = 1024;
        a365397_build(N);
        if (n > a365397_N) a365397_build(n);
        if (n > a365397_N) return 0;
    }
    return (seq_t)a365397_cache[n];
}
