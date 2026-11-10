/*
 * A390180  Powers k^m, m > 1, such that k is in A303946 and A053669(k) <
 *          A006530(k).
 * https://oeis.org/A390180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest k^m (m>1) with k in A303946 (not
 *            squarefree, not a perfect power) and A053669(k) < A006530(k);
 *            computed by a bounded search with limit doubling, then sorting
 * Range:     u8 none, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9986 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A390180_qualifies(uint64_t k)
{
    uint64_t t = k, g = 0, gpf = 1;
    bool nonsqf = false;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (e >= 2) nonsqf = true;
            uint64_t a = g, b = e;
            while (b) { uint64_t r = a % b; a = b; b = r; }
            g = a;
            gpf = p;
        }
    }
    if (t > 1) {
        gpf = t;
        uint64_t a = g, b = 1;
        while (b) { uint64_t r = a % b; a = b; b = r; }
        g = a;
    }
    if (!nonsqf || g != 1) return false;
    uint64_t q = 2;
    for (;;) {
        if (k % q != 0) break;
        ++q;
        while (q > 2) {
            bool pr = true;
            for (uint64_t d = 2; d * d <= q; ++d)
                if (q % d == 0) { pr = false; break; }
            if (pr) break;
            ++q;
        }
    }
    return q < gpf;
}

static int A390180_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static uint64_t *A390180_vals = NULL;
static size_t A390180_cnt = 0;

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    while (A390180_cnt < (size_t)n || A390180_vals == NULL) {
        limit = limit ? limit * 4 : ((uint64_t)1 << 20);
        free(A390180_vals);
        A390180_vals = NULL;
        A390180_cnt = 0;
        size_t cap = 1024;
        uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!arr) return 0;
        for (uint64_t k = 2; k * k <= limit; ++k) {
            if (!A390180_qualifies(k)) continue;
            uint64_t v = k * k;
            for (;;) {
                if (A390180_cnt >= cap) {
                    cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
                    if (!tmp) { free(arr); return 0; }
                    arr = tmp;
                }
                arr[A390180_cnt++] = v;
                if (v > limit / k) break;
                v *= k;
            }
        }
        qsort(arr, A390180_cnt, sizeof(uint64_t), A390180_cmp);
        A390180_vals = arr;
    }
    return (seq_t)A390180_vals[(size_t)n - 1];
}
