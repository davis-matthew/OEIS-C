/*
 * A341060  Numbers k such that A340179(k) is a multiple of k.
 * https://oeis.org/A341060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A340179(k) == 0 (mod k), where
 *            A340179(k) = Sum_{x<k, gcd(x,k)=1} (c mod x), c = k*phi(k)/2;
 *            found by incremental search with memoized terms
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *found_terms = NULL;
static size_t found_cnt = 0, found_cap = 0;
static uint64_t next_k = 1;
static unsigned char *mark_buf = NULL;
static size_t mark_cap = 0;

static int test_k(uint64_t k) {
    if (k == 1) return 1;
    if (mark_cap < k + 1) {
        free(mark_buf);
        mark_cap = (size_t)(k + 1) * 2;
        mark_buf = (unsigned char *)malloc(mark_cap);
        if (!mark_buf) { mark_cap = 0; return 0; }
    }
    unsigned char *m = mark_buf;
    for (uint64_t i = 0; i <= k; i++) m[i] = 0;
    uint64_t t = k, phi = k;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            phi = phi / p * (p - 1);
            for (uint64_t j = p; j < k; j += p) m[j] = 1;
        }
    }
    if (t > 1) {
        phi = phi / t * (t - 1);
        for (uint64_t j = t; j < k; j += t) m[j] = 1;
    }
    uint64_t c = k * phi / 2;
    uint64_t sum = 0;
    if (c < 4294967296ULL) {
        uint32_t c32 = (uint32_t)c;
        for (uint32_t x = 1; x < (uint32_t)k; x++)
            if (!m[x]) sum += c32 % x;
    } else {
        for (uint64_t x = 1; x < k; x++)
            if (!m[x]) sum += c % x;
    }
    return sum % k == 0;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (found_cnt < (size_t)n) {
        uint64_t k = next_k++;
        if (test_k(k)) {
            if (found_cnt == found_cap) {
                found_cap = found_cap ? found_cap * 2 : 64;
                found_terms = (uint64_t *)realloc(found_terms, found_cap * sizeof(uint64_t));
            }
            found_terms[found_cnt++] = k;
        }
    }
    return (seq_t)found_terms[n - 1];
}
