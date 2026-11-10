/*
 * A352268  Integers that need 10 iterations of the map x->A352172(x) to reach
 *          1.
 * https://oeis.org/A352268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m (increasing) such that iterating x ->
 *            A352172(x) (product of cubes of nonzero digits) reaches 1 in
 *            exactly 10 steps; found by search with exact big-number
 *            iteration
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A352268_MAXL 64

static void a352268_mul(uint32_t *l, int *n, uint32_t m) {
    uint64_t carry = 0;
    for (int i = 0; i < *n; i++) {
        uint64_t v = (uint64_t)l[i] * m + carry;
        l[i] = (uint32_t)(v % 1000000000ULL);
        carry = v / 1000000000ULL;
    }
    while (carry && *n < A352268_MAXL) {
        l[(*n)++] = (uint32_t)(carry % 1000000000ULL);
        carry /= 1000000000ULL;
    }
}

static int a352268_mulpow(uint32_t *l, int *n, uint32_t base, int e, int chunk) {
    while (e > 0) {
        int k = e < chunk ? e : chunk;
        uint32_t m = 1;
        for (int i = 0; i < k; i++) m *= base;
        a352268_mul(l, n, m);
        if (*n >= A352268_MAXL) return 0;
        e -= k;
    }
    return 1;
}

static int a352268_check(uint64_t x) {
    int cnt[10];
    memset(cnt, 0, sizeof(cnt));
    while (x > 0) { cnt[x % 10]++; x /= 10; }
    for (int step = 1; step <= 10; step++) {
        int e2 = 3 * (cnt[2] + 2 * cnt[4] + 3 * cnt[8] + cnt[6]);
        int e3 = 3 * (cnt[3] + cnt[6] + 2 * cnt[9]);
        int e5 = 3 * cnt[5];
        int e7 = 3 * cnt[7];
        uint32_t l[A352268_MAXL + 2];
        int n = 1;
        l[0] = 1;
        if (!a352268_mulpow(l, &n, 2, e2, 29)) return 0;
        if (!a352268_mulpow(l, &n, 3, e3, 18)) return 0;
        if (!a352268_mulpow(l, &n, 5, e5, 12)) return 0;
        if (!a352268_mulpow(l, &n, 7, e7, 10)) return 0;
        if (n >= A352268_MAXL - 1) return 0;
        if (n == 1 && l[0] == 1) return step == 10;
        if (step == 10) return 0;
        memset(cnt, 0, sizeof(cnt));
        for (int i = 0; i < n; i++) {
            uint32_t v = l[i];
            if (i == n - 1) {
                while (v > 0) { cnt[v % 10]++; v /= 10; }
            } else {
                for (int j = 0; j < 9; j++) { cnt[v % 10]++; v /= 10; }
            }
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    static uint64_t *terms = 0;
    static size_t count = 0, cap = 0;
    static uint64_t next = 1;
    while (count < (size_t)n) {
        if (a352268_check(next)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[count++] = next;
        }
        next++;
    }
    return (seq_t)terms[(size_t)n - 1];
}
