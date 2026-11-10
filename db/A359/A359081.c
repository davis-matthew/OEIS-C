/*
 * A359081  a(n) is the least number k such that A246600(k) = n, and -1 if no
 *          such k exists.
 * https://oeis.org/A359081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k with A246600(k) = n, found by search:
 *            iterative-deepening DFS over odd k (by prime factorization) with
 *            d(k) >= n; the largest prime factor is handled in bulk by
 *            enumerating feasible low-bit patterns of k
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 128, u64 n <= 142,
 *            u128 n <= 142, big any n
 * Validated: standard, pass, 55 terms through n = 137 (2026-11-10)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

static uint64_t *A359081_pr = NULL;
static uint8_t *A359081_comp = NULL;
static size_t A359081_pc = 0;
static uint64_t A359081_plim = 0;

static void A359081_sieve_to(uint64_t lim)
{
    if (lim <= A359081_plim) return;
    if (lim < 1024) lim = 1024;
    size_t sz = (size_t)(lim / 2 + 1);
    uint8_t *comp = (uint8_t *)calloc(sz, 1);
    if (!comp) return;
    for (uint64_t i = 1; (2 * i + 1) * (2 * i + 1) <= lim; i++) {
        if (comp[i]) continue;
        uint64_t p = 2 * i + 1;
        for (uint64_t m = p * p; m <= lim; m += 2 * p) comp[m / 2] = 1;
    }
    size_t cnt = 0;
    for (uint64_t i = 1; 2 * i + 1 <= lim; i++) if (!comp[i]) cnt++;
    uint64_t *np = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    if (!np) { free(comp); return; }
    size_t c = 0;
    for (uint64_t i = 1; 2 * i + 1 <= lim; i++) if (!comp[i]) np[c++] = 2 * i + 1;
    free(A359081_pr);
    free(A359081_comp);
    A359081_pr = np;
    A359081_comp = comp;
    A359081_pc = cnt;
    A359081_plim = lim;
}

static bool A359081_isp(uint64_t q)
{
    if (q < 3 || (q & 1) == 0) return q == 2;
    if (q <= A359081_plim) return !A359081_comp[q / 2];
    return is_prime_u64(q);
}

static uint64_t A359081_fp[64];
static unsigned A359081_fe[64];
static size_t A359081_fc;
static uint64_t A359081_n, A359081_L, A359081_best;

static uint64_t *A359081_dv = NULL;
static size_t A359081_dcap = 0;
static uint64_t *A359081_sm = NULL;
static size_t A359081_scap = 0;

static size_t A359081_gen_divs(void)
{
    size_t total = 1;
    for (size_t i = 0; i < A359081_fc; i++) total *= (size_t)(A359081_fe[i] + 1);
    if (total > A359081_dcap) {
        free(A359081_dv);
        A359081_dv = (uint64_t *)malloc(total * sizeof(uint64_t));
        A359081_dcap = total;
        free(A359081_sm);
        A359081_sm = (uint64_t *)malloc(total * sizeof(uint64_t));
        A359081_scap = total;
    }
    size_t m = 1;
    A359081_dv[0] = 1;
    for (size_t i = 0; i < A359081_fc; i++) {
        size_t old = m;
        uint64_t v = 1;
        for (unsigned e = 1; e <= A359081_fe[i]; e++) {
            v *= A359081_fp[i];
            for (size_t k = 0; k < old; k++) A359081_dv[m++] = A359081_dv[k] * v;
        }
    }
    return m;
}

static uint64_t A359081_cur_lim(void)
{
    uint64_t l = A359081_L;
    if (A359081_best - 1 < l) l = A359081_best - 1;
    return l;
}

static bool A359081_check_node(uint64_t c, size_t m, uint64_t F)
{
    uint64_t fails = 0;
    for (size_t i = 0; i < m; i++) {
        uint64_t d = A359081_dv[i];
        if ((c & d) != d) { if (++fails > F) return false; }
    }
    return fails == F;
}

static bool A359081_check_cp(uint64_t c, uint64_t p, size_t m, uint64_t F)
{
    uint64_t k = c * p, fails = 0;
    for (size_t i = 0; i < m; i++) {
        uint64_t d = A359081_dv[i];
        uint64_t dp = d * p;
        if ((k & dp) != dp) { if (++fails > F) return false; }
        if ((k & d) != d) { if (++fails > F) return false; }
    }
    return fails == F;
}

/* tail state */
static uint64_t T_c, T_lo, T_hi, T_F, T_mask, T_cinv;
static unsigned T_b;
static size_t T_m, T_ns;

static void A359081_try_p(uint64_t p)
{
    if (p <= T_lo || p > T_hi) return;
    if (!A359081_isp(p)) return;
    if (A359081_check_cp(T_c, p, T_m, T_F)) {
        uint64_t k = T_c * p;
        if (k < A359081_best) {
            A359081_best = k;
            T_hi = (A359081_best - 1) / T_c;
        }
    }
}

static void A359081_leaf(uint64_t s)
{
    uint64_t M = T_mask + 1;
    uint64_t r = (s * T_cinv) & T_mask;
    uint64_t p;
    if (r > T_lo) p = r;
    else p = r + ((T_lo - r) / M + 1) * M;
    while (p <= T_hi) {
        A359081_try_p(p);
        p += M;
    }
}

static void A359081_bitdfs(unsigned i, uint64_t s, uint64_t fails)
{
    if (i == T_b) { A359081_leaf(s); return; }
    A359081_bitdfs(i + 1, s | ((uint64_t)1 << i), fails);
    uint64_t low = ((uint64_t)1 << i) - 1;
    uint64_t add = 0;
    for (size_t k = 0; k < T_ns; k++) {
        uint64_t d = A359081_sm[k];
        if (((d >> i) & 1) && ((d & ~s & low) == 0)) {
            add++;
            if (fails + add > T_F) return;
        }
    }
    A359081_bitdfs(i + 1, s, fails + add);
}

static void A359081_tail(uint64_t c, uint64_t t, uint64_t lo, uint64_t hi)
{
    T_c = c; T_lo = lo; T_hi = hi;
    T_F = 2 * t - A359081_n;
    T_m = A359081_gen_divs();
    uint64_t range = hi - lo;
    if (range <= 512) {
        uint64_t q = lo + 1;
        if ((q & 1) == 0) q++;
        for (; q <= T_hi; q += 2) A359081_try_p(q);
        return;
    }
    unsigned b = 0;
    while (b < 62 && ((uint64_t)1 << b) <= range) b++;
    T_b = b;
    T_mask = ((uint64_t)1 << b) - 1;
    T_ns = 0;
    for (size_t i = 0; i < T_m; i++) {
        uint64_t d = A359081_dv[i];
        if (d > 1 && d <= T_mask) A359081_sm[T_ns++] = d;
    }
    uint64_t x = c;
    for (int it = 0; it < 6; it++) x *= 2 - c * x;
    T_cinv = x;
    A359081_bitdfs(1, 1, 0);
}

static void A359081_dfs(size_t j0, uint64_t c, uint64_t t, uint64_t maxp)
{
    uint64_t n = A359081_n;
    uint64_t lim = A359081_cur_lim();
    if (c > lim) return;
    if (t >= n) {
        size_t m = A359081_gen_divs();
        if (A359081_check_node(c, m, t - n) && c < A359081_best) A359081_best = c;
        lim = A359081_cur_lim();
    }
    uint64_t rem = lim / c;
    uint64_t lastp = maxp;
    for (size_t j = j0; j < A359081_pc; j++) {
        uint64_t p = A359081_pr[j];
        if (p > rem / p) break;
        unsigned r = 0;
        uint64_t q = rem;
        while (q >= p) { q /= p; r++; }
        if (r < 40 && (t << r) < n) return;
        A359081_fp[A359081_fc] = p;
        uint64_t v = c;
        unsigned e = 0;
        while (v <= lim / p) {
            v *= p;
            e++;
            A359081_fe[A359081_fc] = e;
            A359081_fc++;
            A359081_dfs(j + 1, v, t * (e + 1), p);
            A359081_fc--;
            lim = A359081_cur_lim();
        }
        rem = lim / c;
        lastp = p;
    }
    if (2 * t >= n) {
        lim = A359081_cur_lim();
        rem = lim / c;
        if (rem > lastp) A359081_tail(c, t, lastp, rem);
    }
}

static uint64_t A359081_memo[1024];
static bool A359081_known[1024];

static uint64_t A359081_solve(uint64_t n)
{
    for (uint64_t L = 1024;; L *= 2) {
        A359081_sieve_to(isqrt_u64(L) + 2);
        A359081_n = n;
        A359081_L = L;
        A359081_best = UINT64_MAX;
        A359081_fc = 0;
        A359081_dfs(0, 1, 1, 1);
        if (A359081_best != UINT64_MAX) return A359081_best;
        if (L >= ((uint64_t)1 << 61)) return 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0 - (seq_t)1;
    uint64_t r;
    if (n < 1024 && A359081_known[n]) {
        r = A359081_memo[n];
    } else {
        r = A359081_solve((uint64_t)n);
        if (n < 1024) { A359081_memo[n] = r; A359081_known[n] = true; }
    }
    if (r == 0) return (seq_t)0 - (seq_t)1;
    return (seq_t)r;
}
