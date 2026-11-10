/*
 * A292924  a(n) is the least k such that A122953(k) = n.
 * https://oeis.org/A292924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k with A122953(k) = n. Found by search: for each
 *            bit length L in increasing order, a depth-first search over
 *            binary strings of length L that start with 1, trying 0 before 1
 *            at each position. The first string found is the smallest k for
 *            that L. Branches are cut when an upper bound sho...
 * Range:     u8 n <= 21, u16 n <= 82, u32 n <= 288, u64 n <= 288,
 *            u128 n <= 288, big any n
 * Validated: standard, pass, 68 terms through n = 269 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define A292924_HS 8192

static uint64_t A292924_tab[A292924_HS];
static unsigned A292924_stk[4200];
static unsigned A292924_lstk[4200];
static unsigned A292924_sp;
static unsigned A292924_L;
static unsigned A292924_n;
static uint64_t A292924_res;
static unsigned char A292924_bits[72];
static unsigned A292924_ones[73];
static unsigned A292924_seenm[72];

static bool A292924_insert(uint64_t v, unsigned m)
{
    unsigned h = (unsigned)((v * 0x9E3779B97F4A7C15ULL) >> 51) & (A292924_HS - 1);
    while (A292924_tab[h]) {
        if (A292924_tab[h] == v) return false;
        h = (h + 1) & (A292924_HS - 1);
    }
    A292924_tab[h] = v;
    A292924_lstk[A292924_sp] = m;
    A292924_stk[A292924_sp++] = h;
    A292924_seenm[m]++;
    return true;
}

static bool A292924_dfs(unsigned p, uint64_t cur, unsigned c)
{
    unsigned L = A292924_L;
    if (p == L) {
        if (c == A292924_n) { A292924_res = cur; return true; }
        return false;
    }
    /* upper bound on the final count */
    {
        unsigned long long total = c;
        bool enough = false;
        for (unsigned m = 1; m <= L; ++m) {
            unsigned lowend = (p > m - 1) ? p : m - 1;
            if (lowend > L - 1) continue;
            unsigned sLo = lowend - (m - 1);
            unsigned sHi = L - m;
            unsigned long long cand = 0;
            unsigned kHi = (p - 1 < sHi) ? p - 1 : sHi; /* known starts sLo..kHi */
            if (sLo <= kHi) cand += A292924_ones[kHi + 1] - A292924_ones[sLo];
            if (sHi >= p) cand += sHi - p + 1;
            unsigned long long cap = (m - 1 >= 40) ? (1ULL << 40)
                                     : ((1ULL << (m - 1)) - A292924_seenm[m]);
            total += cand < cap ? cand : cap;
            if (total >= A292924_n) { enough = true; break; }
        }
        if (!enough) return false;
    }
    for (unsigned b = 0; b < 2; ++b) {
        A292924_bits[p] = (unsigned char)b;
        A292924_ones[p + 1] = A292924_ones[p] + b;
        uint64_t nc = (cur << 1) | b;
        unsigned mark = A292924_sp;
        unsigned gained = 0;
        for (unsigned m = 1; m <= p + 1; ++m) {
            unsigned s = p + 1 - m;
            if (!A292924_bits[s]) continue;
            uint64_t v = (m >= 64) ? nc : (nc & ((1ULL << m) - 1));
            if (A292924_insert(v, m)) gained++;
        }
        bool ok = false;
        if (c + gained <= A292924_n)
            ok = A292924_dfs(p + 1, nc, c + gained);
        while (A292924_sp > mark) {
            --A292924_sp;
            A292924_tab[A292924_stk[A292924_sp]] = 0;
            A292924_seenm[A292924_lstk[A292924_sp]]--;
        }
        if (ok) return true;
    }
    return false;
}

#define A292924_CACHE 4096
static uint64_t A292924_cache[A292924_CACHE];

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n < A292924_CACHE && A292924_cache[n]) return (seq_t)A292924_cache[n];
    A292924_n = (unsigned)n;
    for (unsigned L = 1; L <= 64; ++L) {
        unsigned long long M = 0;
        for (unsigned l = 1; l <= L; ++l) {
            unsigned long long cap = (l - 1 >= 62) ? (1ULL << 62) : (1ULL << (l - 1));
            unsigned long long avail = L - l + 1;
            M += cap < avail ? cap : avail;
        }
        if (M < n) continue;
        memset(A292924_tab, 0, sizeof(A292924_tab));
        memset(A292924_seenm, 0, sizeof(A292924_seenm));
        A292924_sp = 0;
        A292924_L = L;
        A292924_bits[0] = 1;
        A292924_ones[0] = 0;
        A292924_ones[1] = 1;
        A292924_insert(1, 1);
        if (A292924_dfs(1, 1, 1)) {
            if (n < A292924_CACHE) A292924_cache[n] = A292924_res;
            return (seq_t)A292924_res;
        }
    }
    return (seq_t)0;
}
