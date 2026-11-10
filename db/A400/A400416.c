/*
 * A400416  Indices of record terms of A400415.
 * https://oeis.org/A400416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k at which A400415(k) = (2k - sigma(k))/gcd(tau(k),
 *            2k - sigma(k)) reaches a new maximum. Found by a search that
 *            skips values that cannot be records: when k is neither a square
 *            nor twice a square, sigma(k) and tau(k) are both even, so
 *            A400415(k) <= (k-1)/2. Any k with (k-1)/2 <= the c...
 * Range:     u8 n <= 22, u16 n <= 71, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 65 terms through n = 1796 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *rec_idx = NULL;
static size_t rec_cnt = 0, rec_cap = 0;
static uint64_t rec_cur = 0;          /* last k examined */
static int64_t rec_best = 0;
static int rec_started = 0;
static uint64_t rec_sm = 1;           /* next m with m^2 > rec_cur */
static uint64_t rec_tm = 1;           /* next m with 2*m^2 > rec_cur */

static uint64_t rec_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* sigma and tau of k = m^mult * c, where c is 1 or 2 */
static void rec_sigma_tau(uint64_t m, int mult, int c, uint64_t *sig, uint64_t *tau)
{
    uint64_t s = 1, t = 1;
    int e2 = (c == 2) ? 1 : 0;
    while ((m & 1) == 0) { m >>= 1; e2 += mult; }
    if (e2 > 0) {
        uint64_t pk = 1, sum = 1;
        for (int i = 0; i < e2; ++i) { pk *= 2; sum += pk; }
        s *= sum; t *= (uint64_t)(e2 + 1);
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            e *= mult;
            uint64_t pk = 1, sum = 1;
            for (int i = 0; i < e; ++i) { pk *= p; sum += pk; }
            s *= sum; t *= (uint64_t)(e + 1);
        }
    }
    if (m > 1) {
        uint64_t pk = 1, sum = 1;
        for (int i = 0; i < mult; ++i) { pk *= m; sum += pk; }
        s *= sum; t *= (uint64_t)(mult + 1);
    }
    *sig = s; *tau = t;
}

static void rec_push(uint64_t k)
{
    if (rec_cnt == rec_cap) {
        size_t nc = rec_cap ? rec_cap * 2 : 256;
        uint64_t *p = (uint64_t *)realloc(rec_idx, nc * sizeof(uint64_t));
        if (!p) return;
        rec_idx = p; rec_cap = nc;
    }
    rec_idx[rec_cnt++] = k;
}

static void rec_step(void)
{
    uint64_t A = rec_sm * rec_sm;
    uint64_t B = 2 * rec_tm * rec_tm;
    uint64_t C = rec_cur + 1;
    if (rec_started && rec_best >= 0) {
        uint64_t lim = 2 * (uint64_t)rec_best + 2;
        if (lim > C) C = lim;
    }
    uint64_t k = A;
    int kind = 1;
    if (B < k) { k = B; kind = 2; }
    if (C < k) { k = C; kind = 0; }

    uint64_t sig, tau;
    if (kind == 1)      rec_sigma_tau(rec_sm, 2, 1, &sig, &tau);
    else if (kind == 2) rec_sigma_tau(rec_tm, 2, 2, &sig, &tau);
    else                rec_sigma_tau(k, 1, 1, &sig, &tau);

    int64_t diff = (int64_t)(2 * k) - (int64_t)sig;
    uint64_t ad = diff < 0 ? (uint64_t)(-diff) : (uint64_t)diff;
    uint64_t g = rec_gcd(ad, tau);
    int64_t v = diff / (int64_t)g;
    if (!rec_started || v > rec_best) {
        rec_best = v; rec_started = 1; rec_push(k);
    }
    rec_cur = k;
    while (rec_sm * rec_sm <= rec_cur) ++rec_sm;
    while (2 * rec_tm * rec_tm <= rec_cur) ++rec_tm;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (rec_cnt < (size_t)n) {
        size_t before = rec_cnt;
        rec_step();
        if (rec_cnt == before && rec_cnt == rec_cap && rec_idx == NULL && rec_cap == 0 && rec_started)
            return (seq_t)0;
    }
    return (seq_t)rec_idx[(size_t)n - 1];
}
