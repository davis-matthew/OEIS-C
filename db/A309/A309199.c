/*
 * A309199  Index of n^2 in A111273.
 * https://oeis.org/A309199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A111273(m) = n^2; computed by simulating
 *            A111273 (a(m) = least divisor of m*(m+1)/2 not yet used) until
 *            n^2 appears
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 880 (2026-11-08)
 * Helpers:   isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/isqrt_u64.h"

#define A309199_BLK 32768
#define A309199_MAXF 16
#define A309199_DENSE_MAX ((uint64_t)1 << 26)
#define A309199_SQ_DENSE ((uint64_t)1 << 20)

/* ---------- primes for segmented factorisation ---------- */
static uint64_t *s_pr = NULL;
static size_t s_npr = 0;
static uint64_t s_prlim = 0;

static void s_ensure_primes(uint64_t lim)
{
    if (lim <= s_prlim) return;
    uint64_t L = s_prlim ? s_prlim : 1024;
    while (L < lim) L <<= 1;
    uint8_t *comp = (uint8_t *)calloc((size_t)(L + 1), 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!comp[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    free(s_pr);
    s_pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    s_npr = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (!comp[i]) s_pr[s_npr++] = i;
    free(comp);
    s_prlim = L;
}

/* ---------- block factorisation ---------- */
static uint64_t s_blo = 0;
static bool s_bvalid = false;
static uint64_t s_fp[A309199_BLK * A309199_MAXF];
static uint8_t s_fe[A309199_BLK * A309199_MAXF];
static uint8_t s_fc[A309199_BLK];
static uint64_t s_rem[A309199_BLK];

static void s_build_block(uint64_t lo)
{
    uint64_t hi = lo + A309199_BLK - 1;
    uint64_t r = isqrt_u64(hi);
    s_ensure_primes(r + 1);
    for (size_t i = 0; i < A309199_BLK; i++) {
        s_rem[i] = lo + i;
        s_fc[i] = 0;
    }
    for (size_t k = 0; k < s_npr && s_pr[k] <= r; k++) {
        uint64_t p = s_pr[k];
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t x = st; x <= hi; x += p) {
            size_t i = (size_t)(x - lo);
            uint8_t e = 0;
            while (s_rem[i] % p == 0) { s_rem[i] /= p; e++; }
            size_t c = s_fc[i]++;
            s_fp[i * A309199_MAXF + c] = p;
            s_fe[i * A309199_MAXF + c] = e;
        }
    }
    for (size_t i = 0; i < A309199_BLK; i++) {
        if (s_rem[i] > 1) {
            size_t c = s_fc[i]++;
            s_fp[i * A309199_MAXF + c] = s_rem[i];
            s_fe[i * A309199_MAXF + c] = 1;
        }
    }
    s_blo = lo;
    s_bvalid = true;
}

static void s_factor(uint64_t x, uint64_t *ps, int *es, int *cnt)
{
    if (x < 2) return;
    if (!s_bvalid || x < s_blo || x >= s_blo + A309199_BLK) s_build_block(x);
    size_t i = (size_t)(x - s_blo);
    for (size_t c = 0; c < s_fc[i]; c++) {
        ps[*cnt] = s_fp[i * A309199_MAXF + c];
        es[*cnt] = s_fe[i * A309199_MAXF + c];
        (*cnt)++;
    }
}

/* ---------- seen set: dense bytes for small values, hash for large ---------- */
static uint8_t *s_dense = NULL;
static uint64_t s_dcap = 0;
static uint64_t *s_hash = NULL;
static size_t s_hcap = 0, s_hcnt = 0;

static size_t s_hslot(uint64_t key, size_t cap)
{
    uint64_t h = key * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (cap - 1));
}

static bool s_hash_has(uint64_t key)
{
    if (!s_hcap) return false;
    size_t i = s_hslot(key, s_hcap);
    while (s_hash[i]) {
        if (s_hash[i] == key) return true;
        i = (i + 1) & (s_hcap - 1);
    }
    return false;
}

static void s_hash_add(uint64_t key)
{
    if ((s_hcnt + 1) * 2 > s_hcap) {
        size_t nc = s_hcap ? s_hcap * 2 : 1024;
        uint64_t *nh = (uint64_t *)calloc(nc, sizeof(uint64_t));
        for (size_t j = 0; j < s_hcap; j++) {
            if (s_hash[j]) {
                size_t i = s_hslot(s_hash[j], nc);
                while (nh[i]) i = (i + 1) & (nc - 1);
                nh[i] = s_hash[j];
            }
        }
        free(s_hash);
        s_hash = nh;
        s_hcap = nc;
    }
    size_t i = s_hslot(key, s_hcap);
    while (s_hash[i]) {
        if (s_hash[i] == key) return;
        i = (i + 1) & (s_hcap - 1);
    }
    s_hash[i] = key;
    s_hcnt++;
}

static bool s_is_seen(uint64_t d)
{
    if (d < A309199_DENSE_MAX) return d < s_dcap && s_dense[d];
    return s_hash_has(d);
}

static void s_mark(uint64_t d)
{
    if (d < A309199_DENSE_MAX) {
        if (d >= s_dcap) {
            uint64_t nc = s_dcap ? s_dcap : 1024;
            while (nc <= d) nc <<= 1;
            s_dense = (uint8_t *)realloc(s_dense, (size_t)nc);
            memset(s_dense + s_dcap, 0, (size_t)(nc - s_dcap));
            s_dcap = nc;
        }
        s_dense[d] = 1;
    } else {
        s_hash_add(d);
    }
}

/* ---------- squares found: dense table for small roots, list otherwise ---------- */
static uint64_t *s_sqidx = NULL;
static uint64_t s_sqcap = 0;
static uint64_t *s_bigk = NULL, *s_bigm = NULL;
static size_t s_nbig = 0, s_bigcap = 0;

static void s_record_square(uint64_t k, uint64_t m)
{
    if (k < A309199_SQ_DENSE) {
        if (k >= s_sqcap) {
            uint64_t nc = s_sqcap ? s_sqcap : 64;
            while (nc <= k) nc <<= 1;
            s_sqidx = (uint64_t *)realloc(s_sqidx, (size_t)(nc * sizeof(uint64_t)));
            memset(s_sqidx + s_sqcap, 0, (size_t)((nc - s_sqcap) * sizeof(uint64_t)));
            s_sqcap = nc;
        }
        s_sqidx[k] = m;
    } else {
        if (s_nbig == s_bigcap) {
            s_bigcap = s_bigcap ? s_bigcap * 2 : 64;
            s_bigk = (uint64_t *)realloc(s_bigk, s_bigcap * sizeof(uint64_t));
            s_bigm = (uint64_t *)realloc(s_bigm, s_bigcap * sizeof(uint64_t));
        }
        s_bigk[s_nbig] = k;
        s_bigm[s_nbig] = m;
        s_nbig++;
    }
}

static uint64_t s_lookup(uint64_t k)
{
    if (k < A309199_SQ_DENSE) return k < s_sqcap ? s_sqidx[k] : 0;
    for (size_t i = 0; i < s_nbig; i++)
        if (s_bigk[i] == k) return s_bigm[i];
    return 0;
}

/* ---------- simulation of A111273 ---------- */
static uint64_t s_M = 0;
static uint64_t s_divs[1 << 17];

static void s_step(void)
{
    uint64_t m = ++s_M;
    uint64_t ps[48];
    int es[48];
    int cnt = 0;
    s_factor(m, ps, es, &cnt);
    s_factor(m + 1, ps, es, &cnt);
    for (int i = 0; i < cnt; i++)
        if (ps[i] == 2) es[i]--;
    size_t nd = 1;
    s_divs[0] = 1;
    for (int i = 0; i < cnt; i++) {
        size_t cur = nd;
        uint64_t pw = 1;
        for (int e = 1; e <= es[i]; e++) {
            pw *= ps[i];
            for (size_t j = 0; j < cur; j++)
                s_divs[nd++] = s_divs[j] * pw;
        }
    }
    uint64_t best = 0;
    for (size_t j = 0; j < nd; j++) {
        uint64_t d = s_divs[j];
        if ((best == 0 || d < best) && !s_is_seen(d)) best = d;
    }
    s_mark(best);
    uint64_t k = isqrt_u64(best);
    if (k * k == best) s_record_square(k, m);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t r;
    while ((r = s_lookup((uint64_t)n)) == 0) s_step();
    return (seq_t)r;
}
