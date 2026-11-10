/*
 * A307279  Indices of record values in A050782.
 * https://oeis.org/A307279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. a(n) = n-th index k at which A050782(k) sets a new
 *            record, with a(1) = 0. For each k, A050782(k) = P/k, where P is
 *            the smallest decimal palindrome divisible by k. P is found by
 *            enumerating short palindromes, then by a digit DP over residues
 *            mod k, one palindrome length at a time. Records...
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A307279_MAXD 4096

static unsigned char *A307279_F = NULL;
static size_t A307279_Fcap = 0;

/* smallest palindrome multiple of k (k % 10 != 0); digits MSB first in out; returns length */
static int A307279_smallest_pal(uint64_t k, unsigned char *out)
{
    /* brute force for lengths 1..8 */
    uint64_t p10[20];
    p10[0] = 1;
    for (int i = 1; i < 20; ++i) p10[i] = p10[i - 1] * 10u;
    for (int L = 1; L <= 8; ++L) {
        int S = (L + 1) / 2;
        for (uint64_t h = p10[S - 1]; h < p10[S]; ++h) {
            uint64_t t = (L & 1) ? h / 10u : h, rev = 0;
            while (t) { rev = rev * 10u + t % 10u; t /= 10u; }
            uint64_t pal = h * p10[L - S] + rev;
            if (pal % k == 0) {
                for (int i = L - 1; i >= 0; --i) { out[i] = (unsigned char)(pal % 10u); pal /= 10u; }
                return L;
            }
        }
    }
    /* digit DP for longer lengths */
    uint64_t *pw = (uint64_t *)malloc(sizeof(uint64_t) * A307279_MAXD);
    uint64_t *w = (uint64_t *)malloc(sizeof(uint64_t) * A307279_MAXD);
    pw[0] = 1 % k;
    for (int i = 1; i < A307279_MAXD; ++i) pw[i] = (pw[i - 1] * 10u) % k;
    int result = 0;
    for (int L = 9; L < A307279_MAXD && !result; ++L) {
        int S = (L + 1) / 2;
        for (int s = 0; s < S; ++s) {
            int o = L - 1 - s;
            w[s] = (o == s) ? pw[s] : (pw[s] + pw[o]) % k;
        }
        size_t need = (size_t)(S + 1) * (size_t)k;
        if (need > A307279_Fcap) {
            free(A307279_F);
            A307279_F = (unsigned char *)malloc(need);
            A307279_Fcap = need;
        }
        unsigned char *F = A307279_F;
        memset(F + (size_t)S * k, 0, (size_t)k);
        F[(size_t)S * k] = 1;
        for (int s = S - 1; s >= 1; --s) {
            unsigned char *cur = F + (size_t)s * k, *nx = F + (size_t)(s + 1) * k;
            memset(cur, 0, (size_t)k);
            for (uint64_t r = 0; r < k; ++r) {
                if (!nx[r]) continue;
                uint64_t v = r;
                for (int d = 0; d < 10; ++d) {
                    cur[v] = 1;
                    v += w[s];
                    if (v >= k) v -= k;
                }
            }
        }
        /* greedy reconstruction */
        uint64_t cr = 0;
        bool ok = true;
        for (int s = 0; s < S; ++s) {
            int chosen = -1;
            for (int d = (s == 0 ? 1 : 0); d <= 9; ++d) {
                uint64_t nr = (cr + (uint64_t)d * w[s]) % k;
                uint64_t rq = (k - nr) % k;
                if (F[(size_t)(s + 1) * k + rq]) { chosen = d; cr = nr; break; }
            }
            if (chosen < 0) { ok = false; break; }
            out[s] = (unsigned char)chosen;
            out[L - 1 - s] = (unsigned char)chosen;
        }
        if (ok) result = L;
    }
    free(pw);
    free(w);
    return result;
}

/* multiply big-endian digit array by m; result little-endian in res; returns length */
static int A307279_mul(const unsigned char *d, int len, uint64_t m, unsigned char *res)
{
    uint64_t carry = 0;
    int n = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint64_t v = (uint64_t)d[i] * m + carry;
        res[n++] = (unsigned char)(v % 10u);
        carry = v / 10u;
    }
    while (carry) { res[n++] = (unsigned char)(carry % 10u); carry /= 10u; }
    while (n > 1 && res[n - 1] == 0) --n;
    return n;
}

static int A307279_cmp(const unsigned char *a, int la, const unsigned char *b, int lb)
{
    if (la != lb) return la < lb ? -1 : 1;
    for (int i = la - 1; i >= 0; --i)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

seq_t a(idx_t n)
{
    static idx_t rec[128];
    static int cnt = 0;
    static uint64_t next_k = 1;
    static unsigned char best[A307279_MAXD];
    static int best_len = 0;
    static uint64_t best_k = 0;
    static unsigned char cur[A307279_MAXD];
    static unsigned char x[A307279_MAXD + 32], y[A307279_MAXD + 32];

    if (cnt == 0) { rec[0] = 0; cnt = 1; }
    if (n < 1) return (seq_t)0;
    while ((idx_t)cnt < n && cnt < 128) {
        uint64_t k = next_k++;
        if (k % 10u == 0) continue;
        int L = A307279_smallest_pal(k, cur);
        if (L == 0) continue;
        bool isrec;
        if (best_len == 0) isrec = true;
        else {
            /* P/k > Pb/kb  <=>  P*kb > Pb*k */
            int lx = A307279_mul(cur, L, best_k, x);
            int ly = A307279_mul(best, best_len, k, y);
            isrec = A307279_cmp(x, lx, y, ly) > 0;
        }
        if (isrec) {
            memcpy(best, cur, (size_t)L);
            best_len = L;
            best_k = k;
            rec[cnt++] = (idx_t)k;
        }
    }
    return (seq_t)rec[n - 1];
}
