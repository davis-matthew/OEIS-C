/*
 * A304047  Numbers k such that A003105(k) is divisible by k.
 * https://oeis.org/A304047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over k: k is a term iff A003105(k) == 0 (mod k), where
 *            Sum A003105(n) x^n = E(x^2)E(x^3)/(E(x)E(x^6)), E(x) =
 *            Product_{k>=1}(1-x^k). Coefficients are computed exactly with
 *            fixed-width multiword integers by the sparse pentagonal-number
 *            recurrences, then each is reduced mod k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A304047_terms = 0;
static idx_t A304047_cnt = 0;
static idx_t A304047_L = 0;

static void A304047_add(uint64_t *d, const uint64_t *s, unsigned W) {
    uint64_t c = 0;
    for (unsigned j = 0; j < W; j++) {
        uint64_t x = s[j] + c;
        c = (x < c);
        d[j] += x;
        c |= (d[j] < x);
    }
}

static void A304047_sub(uint64_t *d, const uint64_t *s, unsigned W) {
    uint64_t b = 0;
    for (unsigned j = 0; j < W; j++) {
        uint64_t x = s[j] + b;
        b = (x < b);
        uint64_t old = d[j];
        d[j] = old - x;
        b |= (old < x);
    }
}

/* in place: dp <- dp / E(x^s), ascending */
static void A304047_div(uint64_t *dp, idx_t L, unsigned W, idx_t s) {
    for (idx_t n = 1; n <= L; n++) {
        uint64_t *d = dp + (size_t)n * W;
        for (idx_t j = 1;; j++) {
            idx_t k1 = s * (j * (3 * j - 1) / 2);
            if (k1 > n) break;
            idx_t k2 = s * (j * (3 * j + 1) / 2);
            if (j & 1) {
                A304047_add(d, dp + (size_t)(n - k1) * W, W);
                if (k2 <= n) A304047_add(d, dp + (size_t)(n - k2) * W, W);
            } else {
                A304047_sub(d, dp + (size_t)(n - k1) * W, W);
                if (k2 <= n) A304047_sub(d, dp + (size_t)(n - k2) * W, W);
            }
        }
    }
}

/* in place: dp <- dp * E(x^s), descending */
static void A304047_mul(uint64_t *dp, idx_t L, unsigned W, idx_t s) {
    for (idx_t n = L; n >= 1; n--) {
        uint64_t *d = dp + (size_t)n * W;
        for (idx_t j = 1;; j++) {
            idx_t k1 = s * (j * (3 * j - 1) / 2);
            if (k1 > n) break;
            idx_t k2 = s * (j * (3 * j + 1) / 2);
            if (j & 1) {
                A304047_sub(d, dp + (size_t)(n - k1) * W, W);
                if (k2 <= n) A304047_sub(d, dp + (size_t)(n - k2) * W, W);
            } else {
                A304047_add(d, dp + (size_t)(n - k1) * W, W);
                if (k2 <= n) A304047_add(d, dp + (size_t)(n - k2) * W, W);
            }
        }
    }
}

static void A304047_build(idx_t L) {
    idx_t r = 1;
    while ((r + 1) * (r + 1) <= L) r++;
    /* log2 A003105(L) ~ pi*sqrt(2L/9)/ln 2 ~ 2.14*sqrt(L) */
    idx_t bits = 214 * (r + 1) / 100 + 128;
    unsigned W = (unsigned)(bits / 64 + 1);
    uint64_t *dp = (uint64_t *)calloc((size_t)(L + 1) * W, sizeof(uint64_t));
    dp[0] = 1;
    A304047_div(dp, L, W, 1);
    A304047_div(dp, L, W, 6);
    A304047_mul(dp, L, W, 2);
    A304047_mul(dp, L, W, 3);

    free(A304047_terms);
    A304047_terms = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    A304047_cnt = 0;
    for (idx_t k = 1; k <= L; k++) {
        const uint64_t *d = dp + (size_t)k * W;
        unsigned __int128 rem = 0;
        for (unsigned j = W; j-- > 0;) {
            rem = ((rem << 64) | d[j]) % (unsigned __int128)k;
        }
        if (rem == 0) A304047_terms[A304047_cnt++] = (uint64_t)k;
    }
    free(dp);
    A304047_L = L;
}

seq_t a(idx_t n) {
    if (A304047_terms == 0) A304047_build(40000);
    while (n > A304047_cnt) A304047_build(A304047_L * 2);
    return (seq_t)A304047_terms[n - 1];
}
