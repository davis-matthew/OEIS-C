/*
 * A361227  Irregular triangle T(n, k), n > 0, k = 0..A007814(n), read by
 *          rows: T(n, k) = Sum_{i = n-2^k+1..n} A361144(i).
 * https://oeis.org/A361227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n, k) = Sum_{i = n-2^k+1..n} A361144(i) = P(n) - P(n-2^k),
 *            where P is the prefix sum of A361144; A361144 is built greedily
 *            (smallest unused value whose new window sums are all unused),
 *            rows flattened via cumulative length 2r - popcount(r)
 * Range:     u8 n <= 60, u16 n <= 1020, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 82 terms through n = 7894 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A361227_tab = 0;
static size_t A361227_cap = 0, A361227_cnt = 0;
static uint64_t *A361227_P = 0;      /* prefix sums of A361144 */
static size_t A361227_Pcap = 0;
static size_t A361227_rows = 0;
static uint64_t A361227_minfree = 1;

static unsigned A361227_v2(uint64_t n) {
    unsigned c = 0;
    while ((n & 1) == 0) { c++; n >>= 1; }
    return c;
}

static uint64_t A361227_hash(uint64_t x) {
    x ^= x >> 33; x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static int A361227_has(uint64_t v) {
    if (A361227_cap == 0) return 0;
    size_t mask = A361227_cap - 1;
    size_t i = (size_t)A361227_hash(v) & mask;
    while (A361227_tab[i]) {
        if (A361227_tab[i] == v) return 1;
        i = (i + 1) & mask;
    }
    return 0;
}

static void A361227_raw_insert(uint64_t *tab, size_t cap, uint64_t v) {
    size_t mask = cap - 1;
    size_t i = (size_t)A361227_hash(v) & mask;
    while (tab[i]) {
        if (tab[i] == v) return;
        i = (i + 1) & mask;
    }
    tab[i] = v;
}

static void A361227_insert(uint64_t v) {
    if (A361227_cap == 0 || (A361227_cnt + 1) * 2 > A361227_cap) {
        size_t nc = A361227_cap ? A361227_cap * 2 : 1024;
        uint64_t *nt = (uint64_t *)calloc(nc, sizeof(uint64_t));
        size_t i;
        for (i = 0; i < A361227_cap; i++)
            if (A361227_tab[i]) A361227_raw_insert(nt, nc, A361227_tab[i]);
        free(A361227_tab);
        A361227_tab = nt;
        A361227_cap = nc;
    }
    A361227_raw_insert(A361227_tab, A361227_cap, v);
    A361227_cnt++;
}

static void A361227_extend(size_t r) {
    if (A361227_Pcap < r + 2) {
        size_t nc = (r + 2) * 2;
        uint64_t *np = (uint64_t *)realloc(A361227_P, nc * sizeof(uint64_t));
        A361227_P = np;
        if (A361227_Pcap == 0) A361227_P[0] = 0;
        A361227_Pcap = nc;
    }
    while (A361227_rows < r) {
        size_t n = A361227_rows + 1;
        unsigned K = A361227_v2(n);
        unsigned k;
        uint64_t m = A361227_minfree;
        for (;; m++) {
            int ok = 1;
            if (A361227_has(m)) continue;
            for (k = 1; k <= K; k++) {
                uint64_t s = m + A361227_P[n - 1] - A361227_P[n - ((size_t)1 << k)];
                if (A361227_has(s)) { ok = 0; break; }
            }
            if (ok) break;
        }
        A361227_P[n] = A361227_P[n - 1] + m;
        for (k = 0; k <= K; k++)
            A361227_insert(A361227_P[n] - A361227_P[n - ((size_t)1 << k)]);
        while (A361227_has(A361227_minfree)) A361227_minfree++;
        A361227_rows = n;
    }
}

static uint64_t A361227_cum(uint64_t r) {
    uint64_t pc = 0, x = r;
    while (x) { pc += x & 1; x >>= 1; }
    return 2 * r - pc;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t lo = 1, hi = m;
    uint64_t r, k;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A361227_cum(mid) >= m) hi = mid; else lo = mid + 1;
    }
    r = lo;
    k = m - A361227_cum(r - 1) - 1;
    A361227_extend((size_t)r);
    return (seq_t)(A361227_P[r] - A361227_P[r - ((uint64_t)1 << k)]);
}
