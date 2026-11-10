/*
 * A241080  Near-misses for A094783: values where A137849(n) = n-1.
 * https://oeis.org/A241080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. A number N is a term when exactly one m < N has d_i(m) <
 *            d_i(N) for some i <= min(d(m),d(N)), where d_i is the i-th
 *            smallest divisor. N <= 1000 is checked by brute force. Above
 *            that, N must be a product of primorials (A025487). If m fails
 *            then 2m fails, so does canonical(m), and every fai...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 16, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#define T_SIEVE 2048
static uint32_t t_pr[400];
static int t_np = 0;

#define T_BUF 110000
static uint64_t t_base[T_BUF], t_tmp[T_BUF], t_bufX[T_BUF];

static uint64_t t_terms[512];
static int t_nterms = 0;
static uint64_t t_lim = 0;
static int t_inited = 0;

static uint64_t *t_fam = NULL;
static size_t t_fcnt = 0, t_fcap = 0;
static uint64_t t_flim = 0;
static uint64_t **t_dc = NULL;
static size_t *t_dn = NULL;

static void t_sieve(void)
{
    static bool comp[T_SIEVE + 1];
    for (int i = 2; i <= T_SIEVE; ++i) {
        if (comp[i]) continue;
        t_pr[t_np++] = (uint32_t)i;
        for (int j = i * i; j <= T_SIEVE; j += i) comp[j] = true;
    }
}

/* sorted divisors of x (all prime factors must be < T_SIEVE) */
static size_t t_divs(uint64_t x, uint64_t *out)
{
    size_t cnt = 1;
    out[0] = 1;
    for (int k = 0; k < t_np && x > 1; ++k) {
        uint64_t p = t_pr[k];
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }
        if (e == 0) continue;
        size_t c0 = cnt;
        memcpy(t_base, out, c0 * sizeof(uint64_t));
        uint64_t pw = 1;
        for (int t = 1; t <= e; ++t) {
            pw *= p;
            size_t i = 0, j = 0, o = 0;
            while (i < cnt && j < c0) {
                uint64_t b = t_base[j] * pw;
                if (out[i] <= b) t_tmp[o++] = out[i++];
                else { t_tmp[o++] = b; ++j; }
            }
            while (i < cnt) t_tmp[o++] = out[i++];
            while (j < c0) t_tmp[o++] = t_base[j++] * pw;
            memcpy(out, t_tmp, o * sizeof(uint64_t));
            cnt = o;
        }
    }
    return cnt;
}

static void t_push(uint64_t v)
{
    if (t_fcnt == t_fcap) {
        t_fcap = t_fcap ? t_fcap * 2 : 1024;
        t_fam = (uint64_t *)realloc(t_fam, t_fcap * sizeof(uint64_t));
    }
    t_fam[t_fcnt++] = v;
}

static void t_fgen(int k, uint64_t cur, int maxe)
{
    if (k >= t_np) return;
    uint64_t p = t_pr[k];
    for (int e = 1; e <= maxe; ++e) {
        if (cur > t_flim / p) break;
        cur *= p;
        t_push(cur);
        t_fgen(k + 1, cur, e);
    }
}

static int t_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static const uint64_t *t_getd(size_t k, size_t *cnt)
{
    if (!t_dc[k]) {
        size_t c = t_divs(t_fam[k], t_bufX);
        t_dc[k] = (uint64_t *)malloc(c * sizeof(uint64_t));
        memcpy(t_dc[k], t_bufX, c * sizeof(uint64_t));
        t_dn[k] = c;
    }
    *cnt = t_dn[k];
    return t_dc[k];
}

static bool t_fails(const uint64_t *Dm, size_t dm, const uint64_t *DN, size_t dn)
{
    size_t minl = dm < dn ? dm : dn;
    for (size_t i = 1; i < minl; ++i)
        if (Dm[i] < DN[i]) return true;
    return false;
}

/* same-signature enumeration */
static uint64_t sg_N, sg_m0;
static int sg_K, sg_ex[64], sg_ch[64];
static bool sg_used[400];
static bool sg_bad;
static const uint64_t *sg_DN;
static size_t sg_dn;

static unsigned __int128 sg_rest(int pos, unsigned __int128 v)
{
    int q = 0;
    for (int t = pos; t < sg_K; ++t) {
        while (q < t_np && sg_used[q]) ++q;
        if (q >= t_np) return (unsigned __int128)sg_N + 1;
        for (int e = 0; e < sg_ex[t]; ++e) {
            v *= t_pr[q];
            if (v > sg_N) return (unsigned __int128)sg_N + 1;
        }
        ++q;
    }
    return v;
}

static void sg_dfs(int pos, uint64_t cur)
{
    if (sg_bad) return;
    if (pos == sg_K) {
        if (cur > sg_m0 && cur <= sg_N) {
            size_t d = t_divs(cur, t_bufX);
            if (t_fails(t_bufX, d, sg_DN, sg_dn)) sg_bad = true;
        }
        return;
    }
    unsigned __int128 Rb = sg_rest(pos + 1, 1);
    if (Rb > sg_N) return;
    int start = (pos > 0 && sg_ex[pos] == sg_ex[pos - 1]) ? sg_ch[pos - 1] + 1 : 0;
    for (int q = start; q < t_np; ++q) {
        if (sg_used[q]) continue;
        unsigned __int128 pe = 1;
        for (int e = 0; e < sg_ex[pos]; ++e) {
            pe *= t_pr[q];
            if (pe > sg_N) { pe = (unsigned __int128)sg_N + 1; break; }
        }
        unsigned __int128 nc = (unsigned __int128)cur * pe;
        if (nc > sg_N) break;
        if (Rb > (unsigned __int128)sg_N / nc) break;
        sg_used[q] = true;
        sg_ch[pos] = q;
        unsigned __int128 lb = sg_rest(pos + 1, nc);
        if (lb <= sg_N) sg_dfs(pos + 1, (uint64_t)nc);
        sg_used[q] = false;
        if (sg_bad) return;
    }
}

static bool t_test(size_t j)
{
    uint64_t N = t_fam[j];
    size_t dn;
    const uint64_t *DN = t_getd(j, &dn);
    int cnt = 0;
    uint64_t m0 = 0;
    for (size_t kk = j; kk > 0; --kk) {
        size_t k = kk - 1;
        uint64_t m = t_fam[k];
        if (N - m >= m) break;           /* m <= N/2 */
        size_t dm;
        const uint64_t *Dm = t_getd(k, &dm);
        if (t_fails(Dm, dm, DN, dn)) {
            ++cnt;
            m0 = m;
            if (cnt > 1) return false;
        }
    }
    if (cnt == 0) return false;
    if (m0 % 2 == 0) {
        size_t dh = t_divs(m0 / 2, t_bufX);
        if (t_fails(t_bufX, dh, DN, dn)) return false;
    }
    /* signature of m0 (nonincreasing exponents on consecutive primes) */
    sg_K = 0;
    uint64_t x = m0;
    for (int k = 0; k < t_np && x > 1; ++k) {
        int e = 0;
        while (x % t_pr[k] == 0) { x /= t_pr[k]; ++e; }
        if (e == 0) break;
        sg_ex[sg_K++] = e;
    }
    sg_N = N; sg_m0 = m0; sg_bad = false; sg_DN = DN; sg_dn = dn;
    memset(sg_used, 0, sizeof(sg_used));
    sg_dfs(0, 1);
    return !sg_bad;
}

static void t_brute_small(void)
{
    static unsigned dl[1001][40];
    static unsigned dc[1001];
    for (unsigned i = 1; i <= 1000; ++i)
        for (unsigned j = i; j <= 1000; j += i)
            dl[j][dc[j]++] = i;
    for (unsigned N = 1; N <= 1000; ++N) {
        unsigned fails = 0;
        for (unsigned m = 1; m < N && fails < 2; ++m) {
            unsigned minl = dc[N] < dc[m] ? dc[N] : dc[m];
            for (unsigned i = 0; i < minl; ++i)
                if (dl[m][i] < dl[N][i]) { ++fails; break; }
        }
        if (fails == 1) t_terms[t_nterms++] = N;
    }
}

static void t_free_cache(void)
{
    if (t_dc) {
        for (size_t k = 0; k < t_fcnt; ++k) free(t_dc[k]);
        free(t_dc);
        free(t_dn);
    }
    t_dc = NULL;
    t_dn = NULL;
}

static void t_ensure(int n)
{
    if (!t_inited) {
        t_inited = 1;
        t_sieve();
        t_brute_small();
        t_lim = 1000;
    }
    while (t_nterms < n && t_lim < UINT64_MAX) {
        uint64_t newlim = (t_lim > UINT64_MAX / 10) ? UINT64_MAX : t_lim * 10;
        t_free_cache();
        t_fcnt = 0;
        t_flim = newlim;
        t_push(1);
        t_fgen(0, 1, 64);
        qsort(t_fam, t_fcnt, sizeof(uint64_t), t_cmp);
        t_dc = (uint64_t **)calloc(t_fcnt, sizeof(uint64_t *));
        t_dn = (size_t *)calloc(t_fcnt, sizeof(size_t));
        for (size_t k = 0; k < t_fcnt && t_nterms < 500; ++k) {
            if (t_fam[k] <= t_lim) continue;
            if (t_test(k)) t_terms[t_nterms++] = t_fam[k];
        }
        t_lim = newlim;
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 500) return (seq_t)0;
    t_ensure((int)n);
    if ((int)n > t_nterms) return (seq_t)0;
    return (seq_t)t_terms[n - 1];
}
