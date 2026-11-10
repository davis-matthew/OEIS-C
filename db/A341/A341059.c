/*
 * A341059  Numbers k such that A340179(k) is a square.
 * https://oeis.org/A341059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A340179(k) is a square, where A340179(k)
 *            = Sum_{1<=x<k, gcd(x,k)=1} ((k*phi(k)/2) mod x); computed by a
 *            sequential search with a cache
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A341059_is_sq(uint64_t v) {
    if (v < 2) return 1;
    uint64_t r = 1;
    uint64_t lo = 1, hi = (v < 4294967296ULL) ? v : 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= v / mid) lo = mid; else hi = mid - 1;
    }
    r = lo;
    return r * r == v;
}

static uint64_t A341059_val(uint64_t n, unsigned char *mark) {
    if (n < 2) return 0;
    uint64_t m = n, phi = n;
    for (uint64_t i = 0; i < n; i++) mark[i] = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi = phi / p * (p - 1);
            for (uint64_t j = p; j < n; j += p) mark[j] = 1;
        }
    }
    if (m > 1) {
        phi = phi / m * (m - 1);
        for (uint64_t j = m; j < n; j += m) mark[j] = 1;
    }
    uint64_t c = n * phi / 2;
    uint64_t sum = 0;
    for (uint64_t x = 1; x < n; x++)
        if (!mark[x]) sum += c % x;
    return sum;
}

static idx_t *A341059_terms = NULL;
static size_t A341059_cnt = 0, A341059_cap = 0;
static idx_t A341059_next = 1;
static unsigned char *A341059_buf = NULL;
static size_t A341059_bufcap = 0;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (A341059_cnt < (size_t)n) {
        idx_t k = A341059_next++;
        if (A341059_bufcap < (size_t)k + 1) {
            size_t nc = (size_t)k * 2 + 16;
            free(A341059_buf);
            A341059_buf = (unsigned char *)malloc(nc);
            A341059_bufcap = nc;
        }
        uint64_t v = A341059_val((uint64_t)k, A341059_buf);
        if (A341059_is_sq(v)) {
            if (A341059_cnt == A341059_cap) {
                A341059_cap = A341059_cap ? A341059_cap * 2 : 64;
                A341059_terms = (idx_t *)realloc(A341059_terms, A341059_cap * sizeof(idx_t));
            }
            A341059_terms[A341059_cnt++] = k;
        }
    }
    return (seq_t)A341059_terms[n - 1];
}
