/*
 * A257733  Permutation of natural numbers: a(1) = 1, a(ludic(n)) =
 *          lucky(1+a(n-1)), a(nonludic(n)) = unlucky(a(n)), where ludic(n) =
 *          n-th ludic number A003309, nonludic(n) = n-th nonludic number
 *          A192607 and lucky = A000959, unlucky = A050505.
 * https://oeis.org/A257733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1; for n > 1: if n is ludic (A003309), a(n) = A000959(1 +
 *            a(r - 1)) where r is the rank of n among the ludic numbers;
 *            otherwise a(n) = A050505(a(k)) where n is the k-th nonludic
 *            number. Computed iteratively, with a sieve for the lucky numbers
 * Range:     u8 n <= 36, u16 n <= 1360, u32 n <= 8587, u64 n <= 8587,
 *            u128 n <= 8587, big any n
 * Validated: standard, pass, 99 terms through n = 8287 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void A003309_fw_dec(uint32_t *t, size_t m, size_t p)
{
    while (p <= m) {
        t[p]--;
        p += p & (~p + 1);
    }
}

static size_t A003309_fw_find(const uint32_t *t, size_t m, size_t top, uint64_t k)
{
    size_t pos = 0;
    size_t step;
    for (step = top; step > 0; step >>= 1) {
        size_t nx = pos + step;
        if (nx <= m && t[nx] < k) {
            pos = nx;
            k -= t[nx];
        }
    }
    return pos + 1;
}

static uint64_t *A003309_vals = NULL;
static size_t A003309_cnt = 0;
static uint64_t A003309_built = 0;

static void A003309_build(uint64_t L)
{
    size_t m = (size_t)(L - 1);
    uint32_t *t = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    uint64_t *out = (uint64_t *)malloc((m + 2) * sizeof(uint64_t));
    size_t i, top, cnt = 0;
    uint64_t rem = m;

    t[0] = 0;
    for (i = 1; i <= m; i++) t[i] = (uint32_t)(i & (~i + 1));
    top = 1;
    while (top * 2 <= m) top *= 2;

    out[cnt++] = 1;
    while (rem > 0) {
        size_t p = A003309_fw_find(t, m, top, 1);
        uint64_t k = (uint64_t)p + 1;
        uint64_t idx;
        out[cnt++] = k;
        A003309_fw_dec(t, m, p);
        rem--;
        idx = k;
        while (idx <= rem) {
            size_t q = A003309_fw_find(t, m, top, idx);
            A003309_fw_dec(t, m, q);
            rem--;
            idx += k - 1;
        }
    }
    free(t);
    free(A003309_vals);
    A003309_vals = out;
    A003309_cnt = cnt;
    A003309_built = L;
}

static uint64_t A003309_get(uint64_t n)
{
    while ((uint64_t)A003309_cnt < n) {
        uint64_t L = n * 20 + 100;
        if (L < A003309_built * 2) L = A003309_built * 2;
        A003309_build(L);
    }
    return A003309_vals[n - 1];
}

/* lucky numbers table */
static uint64_t *lk = NULL;
static size_t lk_cnt = 0;
static uint64_t lk_lim = 0;

static void lk_build(uint64_t L)
{
    size_t cnt = (size_t)((L + 1) / 2);
    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t i, idx;
    for (i = 0; i < cnt; i++) arr[i] = 2 * (uint64_t)i + 1;
    for (idx = 1; idx < cnt; idx++) {
        uint64_t step = arr[idx];
        size_t w;
        if (step > cnt) break;
        w = (size_t)step - 1;
        for (i = (size_t)step - 1; i < cnt; i++) {
            if ((i + 1) % step != 0) arr[w++] = arr[i];
        }
        cnt = w;
    }
    free(lk);
    lk = arr;
    lk_cnt = cnt;
    lk_lim = L;
}

static void lk_grow(void)
{
    lk_build(lk_lim ? lk_lim * 2 : ((uint64_t)1 << 18));
}

static uint64_t lucky_get(uint64_t k)
{
    while ((uint64_t)lk_cnt < k) lk_grow();
    return lk[k - 1];
}

static uint64_t unlucky_get(uint64_t k)
{
    for (;;) {
        if (lk_cnt > 0 && lk[lk_cnt - 1] > k + (lk_cnt - 1)) {
            size_t lo = 0, hi = lk_cnt - 1;
            while (lo < hi) {
                size_t mid = (lo + hi) / 2;
                if (lk[mid] > k + mid) hi = mid;
                else lo = mid + 1;
            }
            return k + lo;
        }
        lk_grow();
    }
}

static uint64_t *av = NULL;
static uint64_t av_n = 0;

seq_t a(idx_t n)
{
    if ((uint64_t)n > av_n) {
        uint64_t T = (uint64_t)n < 9000 ? 9000 : (uint64_t)n;
        unsigned char *isl = (unsigned char *)calloc((size_t)T + 1, 1);
        uint64_t i, lc = 0, nc = 0;
        uint64_t *nr = (uint64_t *)malloc(((size_t)T + 1) * sizeof(uint64_t));
        uint64_t *res = (uint64_t *)malloc(((size_t)T + 1) * sizeof(uint64_t));
        for (i = 1;; i++) {
            uint64_t v = A003309_get(i);
            if (v > T) break;
            isl[v] = 1;
        }
        res[0] = 0;
        for (i = 1; i <= T; i++) {
            if (isl[i]) {
                lc++;
                if (i == 1) res[i] = 1;
                else res[i] = lucky_get(1 + res[lc - 1]);
            } else {
                nc++;
                res[i] = unlucky_get(res[nc]);
            }
        }
        (void)nr;
        free(nr);
        free(isl);
        free(av);
        av = res;
        av_n = T;
    }
    return (seq_t)av[n];
}
