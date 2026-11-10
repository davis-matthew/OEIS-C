/*
 * A394918  Least integer k such that A394475(k) = n.
 * https://oeis.org/A394918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A394475(k) = n, where A394475(k) =
 *            number of distinct primes p not dividing k with p = d +- 1 for
 *            some divisor d of k (search over k, memoized)
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A394918_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1ULL) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static int A394918_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static unsigned A394918_a394475(uint64_t n)
{
    static uint64_t cand[8192];
    size_t m = 0;
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i) continue;
        uint64_t ds[2];
        int nd = 1;
        ds[0] = i;
        if (n / i != i) ds[nd++] = n / i;
        for (int j = 0; j < nd; ++j) {
            uint64_t d = ds[j];
            uint64_t c;
            c = d - 1;
            if (c >= 2 && (n % c) != 0 && A394918_isprime(c) && m < 8192) cand[m++] = c;
            c = d + 1;
            if ((n % c) != 0 && A394918_isprime(c) && m < 8192) cand[m++] = c;
        }
    }
    if (m == 0) return 0;
    qsort(cand, m, sizeof(uint64_t), A394918_cmp);
    unsigned cnt = 1;
    for (size_t i = 1; i < m; ++i)
        if (cand[i] != cand[i - 1]) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    enum { TAB = 256 };
    static uint64_t first[TAB];
    static uint64_t next_k = 1;
    if (n == 0 || n >= TAB) return (seq_t)0;
    while (first[n] == 0) {
        uint64_t k = next_k;
        next_k = (k == 1) ? 2 : k + 2;
        unsigned v = A394918_a394475(k);
        if (v < TAB && first[v] == 0) first[v] = k;
    }
    return (seq_t)first[n];
}
