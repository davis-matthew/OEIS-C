/*
 * A300861  Records in A300858.
 * https://oeis.org/A300861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th strict record value of A300858(m) = (m - phi(m) -
 *            d(m) + 1) - 2*N_in(m), scanning m = 1, 2, 3, ...; found by a
 *            direct search, cached in static state
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static long long *rec_vals = NULL;
static size_t rec_len = 0, rec_cap = 0;
static uint64_t scan_n = 0;      /* last m processed */
static long long best_val = 0;
static bool have_best = false;

static void rec_smooth_count(size_t idx, uint64_t prod, uint64_t n,
                             const uint64_t *pr, size_t cnt, long long *out) {
    if (idx == cnt) { (*out)++; return; }
    uint64_t p = pr[idx];
    uint64_t v = prod;
    rec_smooth_count(idx + 1, v, n, pr, cnt, out);
    while (v <= n / p) {
        v *= p;
        rec_smooth_count(idx + 1, v, n, pr, cnt, out);
    }
}

static long long rec_A300858(uint64_t n) {
    uint64_t pr[24];
    size_t cnt = 0;
    uint64_t m = n;
    long long phi = (long long)n;
    long long d = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[cnt++] = p;
            phi -= phi / (long long)p;
            d *= (e + 1);
        }
    }
    if (m > 1) {
        pr[cnt++] = m;
        phi -= phi / (long long)m;
        d *= 2;
    }
    long long smooth = 0;
    rec_smooth_count(0, 1, n, pr, cnt, &smooth);
    long long nin = smooth - d;
    long long tot = (long long)n - phi - d + 1;
    return tot - 2 * nin;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    while (rec_len < (size_t)n) {
        ++scan_n;
        long long v = rec_A300858(scan_n);
        if (!have_best || v > best_val) {
            have_best = true;
            best_val = v;
            if (rec_len == rec_cap) {
                rec_cap = rec_cap ? rec_cap * 2 : 64;
                rec_vals = (long long *)realloc(rec_vals, rec_cap * sizeof(long long));
            }
            rec_vals[rec_len++] = v;
        }
    }
    return (seq_t)rec_vals[(size_t)n - 1];
}
