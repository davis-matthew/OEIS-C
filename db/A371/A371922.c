/*
 * A371922  Indices of records in A371921: numbers k such that A371921(k) >
 *          A371921(m) for all m < k.
 * https://oeis.org/A371922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k with A371921(k) > A371921(m) for all m < k, where
 *            A371921(k) counts iterations of x -> sigma(x)-2x until
 *            nonpositive (0 if cyclic); found by a segmented sigma sieve scan
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *r_primes = 0;
static size_t r_np = 0;
static uint64_t r_plim = 1;

static void r_ensure_primes(uint64_t lim)
{
    if (lim <= r_plim && r_primes) return;
    uint64_t L = r_plim * 2;
    if (L < lim) L = lim;
    if (L < 1000) L = 1000;
    char *comp = (char *)calloc(L + 1, 1);
    uint32_t *np = (uint32_t *)malloc((L / 2 + 10) * sizeof(uint32_t));
    size_t c = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!comp[i]) {
            np[c++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    free(comp);
    free(r_primes);
    r_primes = np;
    r_np = c;
    r_plim = L;
}

static uint64_t r_isqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    r = lo;
    return r;
}

static uint64_t r_sigma_td(uint64_t x)
{
    r_ensure_primes(r_isqrt(x) + 2);
    uint64_t sg = 1;
    for (size_t i = 0; i < r_np; i++) {
        uint64_t p = r_primes[i];
        if (p * p > x) break;
        if (x % p == 0) {
            uint64_t t = 1, pw = 1;
            while (x % p == 0) { x /= p; pw *= p; t += pw; }
            sg *= t;
        }
    }
    if (x > 1) sg *= (x + 1);
    return sg;
}

static uint8_t *r_cnt = 0;
static uint64_t r_cap = 0;
static uint64_t r_next = 1;      /* next k to scan */
static uint64_t *r_recs = 0;
static size_t r_nrec = 0, r_reccap = 0;
static int r_best = -1;

/* steps from x, using filled counts for x < r_next */
static int r_steps(uint64_t x)
{
    int c = 0;
    for (int it = 0; it < 200; it++) {
        if (x < r_next) {
            if (r_cnt[x] == 0) return 0;
            return c + r_cnt[x];
        }
        uint64_t s = r_sigma_td(x);
        c++;
        if (s <= 2 * x) return c;
        x = s - 2 * x;
    }
    return 0;
}

static void r_scan_block(void)
{
    const uint64_t B = 1ULL << 18;
    uint64_t lo = r_next, hi = lo + B;
    if (hi + 1 > r_cap) {
        uint64_t nc = r_cap ? r_cap * 2 : (1ULL << 20);
        while (nc < hi + 1) nc *= 2;
        r_cnt = (uint8_t *)realloc(r_cnt, nc);
        r_cap = nc;
    }
    r_ensure_primes(r_isqrt(hi) + 2);
    uint64_t *sig = (uint64_t *)malloc(B * sizeof(uint64_t));
    uint64_t *rem = (uint64_t *)malloc(B * sizeof(uint64_t));
    for (uint64_t i = 0; i < B; i++) { sig[i] = 1; rem[i] = lo + i; }
    for (size_t i = 0; i < r_np; i++) {
        uint64_t p = r_primes[i];
        if (p * p >= hi) break;
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t m = st; m < hi; m += p) {
            uint64_t v = rem[m - lo], t = 1, pw = 1;
            while (v % p == 0) { v /= p; pw *= p; t += pw; }
            sig[m - lo] *= t;
            rem[m - lo] = v;
        }
    }
    for (uint64_t i = 0; i < B; i++) {
        uint64_t k = lo + i;
        uint64_t s = sig[i];
        if (rem[i] > 1) s *= (rem[i] + 1);
        int c;
        if (s <= 2 * k) c = 1;
        else {
            r_next = k;  /* only values < k are filled */
            c = r_steps(s - 2 * k);
            if (c != 0) c += 1;
        }
        r_cnt[k] = (uint8_t)c;
        r_next = k + 1;
        if (c > r_best) {
            r_best = c;
            if (r_nrec == r_reccap) {
                r_reccap = r_reccap ? r_reccap * 2 : 64;
                r_recs = (uint64_t *)realloc(r_recs, r_reccap * sizeof(uint64_t));
            }
            r_recs[r_nrec++] = k;
        }
    }
    free(sig);
    free(rem);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (r_nrec < (size_t)n) r_scan_block();
    return (seq_t)r_recs[n - 1];
}
