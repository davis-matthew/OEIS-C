/*
 * A372111  Partial sums of A124652.
 * https://oeis.org/A372111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A124652(k), where A124652(1)=1, A124652(2)=2
 *            and A124652(m) for m > 2 is the smallest unused k with rad(k) |
 *            a(m-1). Each step is a search: first scan just above the
 *            smallest unused number, then fall back to listing the numbers
 *            built only from the primes of the current partial...
 * Range:     u8 n <= 19, u16 n <= 300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9855 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ---------- hash set of used values (uint64, key 0 = empty) ---------- */
static uint64_t *A372111_hs = NULL;
static size_t A372111_hcap = 0, A372111_hcnt = 0;

static size_t A372111_hidx(uint64_t k, size_t cap) {
    uint64_t h = k * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(cap - 1));
}

static bool A372111_used(uint64_t k) {
    size_t i = A372111_hidx(k, A372111_hcap);
    while (A372111_hs[i]) {
        if (A372111_hs[i] == k) return true;
        i = (i + 1) & (A372111_hcap - 1);
    }
    return false;
}

static void A372111_insert_raw(uint64_t *tab, size_t cap, uint64_t k) {
    size_t i = A372111_hidx(k, cap);
    while (tab[i]) {
        if (tab[i] == k) return;
        i = (i + 1) & (cap - 1);
    }
    tab[i] = k;
}

static void A372111_add(uint64_t k) {
    if (2 * (A372111_hcnt + 1) > A372111_hcap) {
        size_t nc = A372111_hcap ? A372111_hcap * 2 : 4096;
        uint64_t *nt = (uint64_t *)calloc(nc, sizeof(uint64_t));
        for (size_t i = 0; i < A372111_hcap; i++)
            if (A372111_hs[i]) A372111_insert_raw(nt, nc, A372111_hs[i]);
        free(A372111_hs);
        A372111_hs = nt;
        A372111_hcap = nc;
    }
    A372111_insert_raw(A372111_hs, A372111_hcap, k);
    A372111_hcnt++;
}

/* ---------- factoring (distinct primes) ---------- */
static uint64_t A372111_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}
static uint64_t A372111_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}
static uint64_t A372111_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m; b %= m;
    while (e) {
        if (e & 1) r = A372111_mulmod(r, b, m);
        b = A372111_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}
static bool A372111_isprime(uint64_t n) {
    static const uint64_t bs[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n % bs[i] == 0) return n == bs[i];
    }
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = A372111_powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A372111_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}
static uint64_t A372111_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (A372111_mulmod(x, x, n) + c) % n;
            y = (A372111_mulmod(y, y, n) + c) % n;
            y = (A372111_mulmod(y, y, n) + c) % n;
            d = A372111_gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}
static void A372111_factor(uint64_t n, uint64_t *pr, int *np) {
    if (n == 1) return;
    for (uint64_t p = 2; p < 1000 && p * p <= n; p++) {
        if (n % p == 0) {
            pr[(*np)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n == 1) return;
    if (A372111_isprime(n)) {
        for (int i = 0; i < *np; i++) if (pr[i] == n) return;
        pr[(*np)++] = n;
        return;
    }
    uint64_t d = A372111_rho(n);
    uint64_t e = n / d;
    A372111_factor(d, pr, np);
    while (e % d == 0) e /= d;
    A372111_factor(e, pr, np);
}

/* rad(k) | S ? */
static bool A372111_radok(uint64_t k, uint64_t S) {
    for (;;) {
        if (k == 1) return true;
        uint64_t g = A372111_gcd(k, S);
        if (g == 1) return false;
        k /= g;
    }
}

/* ---------- DFS over numbers built from the primes of S ---------- */
static uint64_t A372111_P[64];
static int A372111_np = 0;
static uint64_t A372111_best, A372111_B, A372111_lo64;

static void A372111_dfs(int idx, uint64_t v) {
    if (v >= A372111_lo64 && v < A372111_best && !A372111_used(v)) A372111_best = v;
    for (int i = idx; i < A372111_np; i++) {
        uint64_t p = A372111_P[i];
        uint64_t lim = A372111_B;
        if (A372111_best - 1 < lim) lim = A372111_best - 1;
        if (v > lim / p) break;
        A372111_dfs(i, v * p);
    }
}

static int A372111_cmp(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

/* ---------- main ---------- */
static seq_t *A372111_sums = NULL;
static size_t A372111_cnt = 0, A372111_cap = 0;
static uint64_t A372111_S = 0, A372111_lo = 3;

seq_t a(idx_t n) {
    if (A372111_cap == 0) {
        A372111_cap = 1024;
        A372111_sums = mem_new(seq_t, A372111_cap);
        A372111_add(1);
        A372111_add(2);
        A372111_sums[1] = 1;
        A372111_sums[2] = 3;
        A372111_cnt = 2;
        A372111_S = 3;
        A372111_lo = 3;
    }
    while (A372111_cnt < (size_t)n) {
        if (A372111_cnt + 1 >= A372111_cap) {
            A372111_cap *= 2;
            A372111_sums = mem_resize(seq_t, A372111_sums, A372111_cap);
        }
        uint64_t S = A372111_S;
        while (A372111_used(A372111_lo)) A372111_lo++;
        uint64_t k = 0;
        for (uint64_t c = A372111_lo; c < A372111_lo + 64; c++) {
            if (!A372111_used(c) && A372111_radok(c, S)) { k = c; break; }
        }
        if (!k) {
            A372111_np = 0;
            A372111_factor(S, A372111_P, &A372111_np);
            qsort(A372111_P, (size_t)A372111_np, sizeof(uint64_t), A372111_cmp);
            A372111_lo64 = A372111_lo;
            A372111_B = A372111_lo > (UINT64_MAX / 4) ? UINT64_MAX : 4 * A372111_lo;
            for (;;) {
                A372111_best = UINT64_MAX;
                A372111_dfs(0, 1);
                if (A372111_best != UINT64_MAX) { k = A372111_best; break; }
                if (A372111_B > UINT64_MAX / 4) A372111_B = UINT64_MAX;
                else A372111_B *= 4;
            }
        }
        A372111_add(k);
        A372111_S = S + k;
        A372111_sums[A372111_cnt + 1] = A372111_sums[A372111_cnt] + (seq_t)k;
        A372111_cnt++;
    }
    return A372111_sums[(size_t)n];
}
