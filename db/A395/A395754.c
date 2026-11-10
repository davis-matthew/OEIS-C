/*
 * A395754  Number of k < m^2 such that both rad(k) and rad(k+1) divide m,
 *          where rad = A007947 and m = A005117(n).
 * https://oeis.org/A395754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k < m^2 : rad(k) | m and rad(k+1) | m}, m = A005117(n);
 *            computed by enumerating all numbers <= m^2 built from the primes
 *            of m, sorting them, and counting adjacent pairs differing by 1
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9753 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool c395754_sqfree(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static uint64_t c395754_nth_sqfree(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t sz = 0, cap = 0;
    if (n > sz) {
        if (n > cap) {
            idx_t nc = cap ? cap : 64;
            while (nc < n) nc *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
            cap = nc;
        }
        uint64_t k = sz ? cache[sz - 1] : 0;
        for (idx_t i = sz; i < n; i++) {
            do { k++; } while (!c395754_sqfree(k));
            cache[i] = k;
        }
        sz = n;
    }
    return cache[n - 1];
}

static seq_t *c395754_buf = 0;
static size_t c395754_len = 0, c395754_cap = 0;
static uint64_t c395754_pr[24];
static int c395754_np = 0;
static seq_t c395754_limit;

static void c395754_push(seq_t v) {
    if (c395754_len == c395754_cap) {
        size_t nc = c395754_cap ? c395754_cap * 2 : 256;
        c395754_buf = mem_resize(seq_t, c395754_buf, nc);
        c395754_cap = nc;
    }
    c395754_buf[c395754_len++] = v;
}

static void c395754_rec(int i, seq_t cur) {
    if (i == c395754_np) {
        c395754_push(cur);
        return;
    }
    seq_t p = (seq_t)c395754_pr[i];
    seq_t lim_div = c395754_limit / p;
    for (;;) {
        c395754_rec(i + 1, cur);
        if (cur > lim_div) break;
        cur = cur * p;
    }
}

seq_t a(idx_t n) {
    uint64_t m = c395754_nth_sqfree(n);
    c395754_np = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            c395754_pr[c395754_np++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) c395754_pr[c395754_np++] = t;

    c395754_limit = (seq_t)m * (seq_t)m;
    c395754_len = 0;
    c395754_rec(0, (seq_t)1);

    /* shell sort */
    size_t N = c395754_len;
    size_t gap = 1;
    while (gap < N / 3) gap = gap * 3 + 1;
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (size_t i = gap; i < N; i++) {
            seq_t v = c395754_buf[i];
            size_t j = i;
            while (j >= gap && c395754_buf[j - gap] > v) {
                c395754_buf[j] = c395754_buf[j - gap];
                j -= gap;
            }
            c395754_buf[j] = v;
        }
        if (gap == 1) break;
    }

    seq_t cnt = 0;
    for (size_t i = 1; i < N; i++) {
        if (c395754_buf[i] == c395754_buf[i - 1] + 1) cnt++;
    }
    return cnt;
}
