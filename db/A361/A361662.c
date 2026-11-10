/*
 * A361662  Least number k >= 1 such that A074206(k) is divisible by n.
 * https://oeis.org/A361662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A025487(A361663(n)); computed by searching A025487
 *            numbers k in increasing bound windows for the least k with n |
 *            A074206(k), where A074206(k) = sum_{j>=1} sum_{i=0..j}
 *            (-1)^(j-i) C(j,i) prod_t C(e_t+i-1,e_t), evaluated mod n from
 *            the prime signature of k
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 394, u64 n <= 8152,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9394 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t A361662_n;
static uint64_t *A361662_tab;
static size_t A361662_N;
static uint64_t A361662_pr[400];
static int A361662_np = 0;
static int A361662_ex[400];
static seq_t A361662_L;
static seq_t A361662_best;
static bool A361662_have;

static void A361662_init_primes(void)
{
    if (A361662_np) return;
    uint64_t c = 2;
    while (A361662_np < 400) {
        bool ok = true;
        for (int i = 0; i < A361662_np && A361662_pr[i] * A361662_pr[i] <= c; i++)
            if (c % A361662_pr[i] == 0) { ok = false; break; }
        if (ok) A361662_pr[A361662_np++] = c;
        c++;
    }
}

static uint64_t A361662_C(size_t a, size_t b)
{
    return A361662_tab[a * (A361662_N + 1) + b];
}

static bool A361662_eval(int d)
{
    uint64_t n = A361662_n;
    int om = 0;
    for (int t = 0; t < d; t++) om += A361662_ex[t];
    if (om == 0) return (1 % n) == 0;
    uint64_t *P = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(om + 1));
    for (int i = 0; i <= om; i++) {
        unsigned __int128 pr = 1 % n;
        for (int t = 0; t < d; t++) {
            int e = A361662_ex[t];
            size_t a = (size_t)(e + i - 1);
            if (i == 0) { pr = 0; break; }
            pr = (pr * A361662_C(a, (size_t)e)) % n;
        }
        P[i] = (uint64_t)pr;
    }
    uint64_t total = 0;
    for (int j = 1; j <= om; j++) {
        uint64_t g = 0;
        for (int i = 0; i <= j; i++) {
            uint64_t term = (uint64_t)(((unsigned __int128)A361662_C((size_t)j, (size_t)i) * P[i]) % n);
            if (((j - i) & 1) == 0) g = (uint64_t)(((unsigned __int128)g + term) % n);
            else g = (uint64_t)(((unsigned __int128)g + n - term) % n);
        }
        total = (uint64_t)(((unsigned __int128)total + g) % n);
    }
    free(P);
    return total == 0;
}

static void A361662_dfs(int d, seq_t k, int maxe)
{
    if (A361662_have && k >= A361662_best) return;
    if (A361662_eval(d)) {
        if (!A361662_have || k < A361662_best) { A361662_best = k; A361662_have = true; }
        return;
    }
    if (d >= 399) return;
    seq_t p = (seq_t)A361662_pr[d];
    seq_t kk = k;
    for (int e = 1; e <= maxe; e++) {
        if (kk > A361662_L / p) break;
        kk = kk * p;
        if (A361662_have && kk >= A361662_best) break;
        A361662_ex[d] = e;
        A361662_dfs(d + 1, kk, e);
    }
}

seq_t a(idx_t n)
{
    A361662_init_primes();
    A361662_n = (uint64_t)n;
    seq_t L = 64;
    for (;;) {
        int bits = 0;
        seq_t t = L;
        while (t > 0) { t >>= 1; bits++; }
        size_t N = (size_t)(2 * bits + 4);
        A361662_N = N;
        A361662_tab = (uint64_t *)malloc(sizeof(uint64_t) * (N + 1) * (N + 1));
        for (size_t x = 0; x <= N; x++) {
            for (size_t y = 0; y <= N; y++) {
                uint64_t v;
                if (y == 0) v = 1 % A361662_n;
                else if (x == 0) v = 0;
                else v = (uint64_t)(((unsigned __int128)A361662_tab[(x - 1) * (N + 1) + y - 1]
                                     + (y <= x - 1 ? A361662_tab[(x - 1) * (N + 1) + y] : 0)) % A361662_n);
                A361662_tab[x * (N + 1) + y] = v;
            }
        }
        A361662_L = L;
        A361662_have = false;
        A361662_best = 0;
        A361662_dfs(0, (seq_t)1, 1000);
        free(A361662_tab);
        A361662_tab = NULL;
        if (A361662_have) return A361662_best;
        seq_t nl = L * 4;
        if (nl <= L) return (seq_t)0;
        L = nl;
    }
}
