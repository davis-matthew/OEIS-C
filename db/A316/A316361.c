/*
 * A316361  FDH numbers of strict integer partitions such that not every
 *          distinct subset has a different average.
 * https://oeis.org/A316361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th m whose FDH set {y_i} (m = prod f(y_i),
 *            f=A050376) has two distinct nonempty subsets with equal average
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *g_terms = NULL;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_scanned = 1;
static uint64_t g_lim = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

static uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool has_equal_avg(const uint64_t *y, int k) {
    size_t total = (size_t)1 << k;
    uint64_t *keys = (uint64_t *)malloc(sizeof(uint64_t) * total);
    size_t c = 0;
    for (size_t mask = 1; mask < total; mask++) {
        uint64_t s = 0, sz = 0;
        for (int i = 0; i < k; i++)
            if (mask >> i & 1) { s += y[i]; sz++; }
        uint64_t g = gcd64(s, sz);
        keys[c++] = (s / g) * 64 + (sz / g);
    }
    qsort(keys, c, sizeof(uint64_t), cmp_u64);
    bool found = false;
    for (size_t i = 1; i < c; i++)
        if (keys[i] == keys[i - 1]) { found = true; break; }
    free(keys);
    return found;
}

static void extend(void) {
    uint64_t L = g_lim ? g_lim * 2 : 4096;
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    size_t fcap = 1024, fn = 0;
    uint64_t *fd = (uint64_t *)malloc(sizeof(uint64_t) * fcap);
    for (uint64_t p = 2; p <= L; p++) {
        if (spf[p] != p) continue;
        uint64_t q = p;
        while (q <= L) {
            if (fn == fcap) { fcap *= 2; fd = (uint64_t *)realloc(fd, sizeof(uint64_t) * fcap); }
            fd[fn++] = q;
            if (q > L / q) break;
            q = q * q;
        }
    }
    qsort(fd, fn, sizeof(uint64_t), cmp_u64);

    for (uint64_t m = g_scanned + 1; m <= L; m++) {
        uint64_t y[64];
        int k = 0;
        uint64_t r = m;
        while (r > 1) {
            uint64_t p = spf[r];
            int e = 0;
            while (r % p == 0) { r /= p; e++; }
            uint64_t q = p;
            while (e) {
                if (e & 1) {
                    size_t lo = 0, hi = fn;
                    while (lo < hi) {
                        size_t mid = (lo + hi) / 2;
                        if (fd[mid] < q) lo = mid + 1; else hi = mid;
                    }
                    y[k++] = lo + 1;
                }
                e >>= 1;
                if (e) q = q * q;
            }
        }
        if (k >= 3 && has_equal_avg(y, k)) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 256;
                g_terms = (uint64_t *)realloc(g_terms, sizeof(uint64_t) * g_cap);
            }
            g_terms[g_cnt++] = m;
        }
    }
    g_scanned = L;
    g_lim = L;
    free(spf);
    free(fd);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (g_cnt < (size_t)n) extend();
    return (seq_t)g_terms[(size_t)n - 1];
}
