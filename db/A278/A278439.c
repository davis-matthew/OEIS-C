/*
 * A278439  Numbers k such that k | A047842(k).
 * https://oeis.org/A278439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini
 * Formula:   a(n) = n-th k (ascending) with k | A047842(k); search over
 *            digit-count vectors (k<10^6 by brute force, longer k via m =
 *            A047842(k)/k with bounded m range), finite sequence
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A278439_u128;

static A278439_u128 A278439_res[512];
static int A278439_cnt = 0;
static bool A278439_done = false;
static int A278439_c[10];
static int A278439_d;

static void A278439_add(A278439_u128 k)
{
    for (int i = 0; i < A278439_cnt; i++)
        if (A278439_res[i] == k) return;
    if (A278439_cnt < 512) A278439_res[A278439_cnt++] = k;
}

static void A278439_process(void)
{
    const int *c = A278439_c;
    int d = A278439_d;
    if (c[0] >= d) return;
    A278439_u128 D = 0;
    int S = 0;
    for (int i = 0; i < 10; i++) {
        if (c[i] == 0) continue;
        A278439_u128 mult = (c[i] < 10) ? 100 : 1000;
        D = D * mult + (A278439_u128)(c[i] * 10 + i);
        S += i * c[i];
    }
    /* min and max k with this digit multiset */
    int cc[10];
    for (int i = 0; i < 10; i++) cc[i] = c[i];
    int lead = 1;
    while (lead < 10 && cc[lead] == 0) lead++;
    if (lead >= 10) return;
    cc[lead]--;
    A278439_u128 mink = (A278439_u128)lead;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < cc[i]; j++) mink = mink * 10 + (A278439_u128)i;
    A278439_u128 maxk = 0;
    for (int i = 9; i >= 0; i--)
        for (int j = 0; j < c[i]; j++) maxk = maxk * 10 + (A278439_u128)i;
    A278439_u128 mlo = (D + maxk - 1) / maxk;
    A278439_u128 mhi = D / mink;
    if (mhi < mlo) return;
    if (mhi - mlo > 400) mhi = mlo + 400;
    /* residue filter mod 9: m*S == D (mod 9) */
    int Dm = (int)(D % 9);
    bool ok[9];
    bool any = false;
    for (int r = 0; r < 9; r++) {
        ok[r] = (((r * S) % 9) == Dm);
        if (ok[r]) any = true;
    }
    if (!any) return;
    int r = (int)(mlo % 9);
    bool small = (D <= (A278439_u128)UINT64_MAX);
    uint64_t D64 = (uint64_t)D;
    for (A278439_u128 m = mlo; m <= mhi; m++) {
        if (ok[r]) {
            bool div;
            if (small) div = (D64 % (uint64_t)m) == 0;
            else div = (D % m) == 0;
            if (div) {
                A278439_u128 k = D / m;
                int cnt[10] = {0};
                A278439_u128 t = k;
                int len = 0;
                while (t > 0) { cnt[(int)(t % 10)]++; t /= 10; len++; }
                bool same = (len == d);
                for (int i = 0; same && i < 10; i++)
                    if (cnt[i] != c[i]) same = false;
                if (same) A278439_add(k);
            }
        }
        if (++r == 9) r = 0;
    }
}

static int A278439_bound(int i, int s)
{
    if (s == 0) return 0;
    int r = 10 - i;
    if (r <= 0) return -1000;
    int best = 0;
    for (int t = 0; t <= r && 10 * t <= s; t++) {
        int a = r - t;
        int b = s - 10 * t;
        int cand = 3 * t + 2 * (a < b ? a : b);
        if (cand > best) best = cand;
    }
    return best;
}

static void A278439_dfs(int i, int s, int L)
{
    if (L + A278439_bound(i, s) < A278439_d) return;
    if (i == 10) {
        if (s == 0) A278439_process();
        return;
    }
    if (i == 9) {
        A278439_c[9] = s;
        A278439_process_dummy:
        ;
        int add = s ? (s < 10 ? 2 : 3) : 0;
        if (L + add >= A278439_d) A278439_process();
        return;
    }
    for (int cnt = 0; cnt <= s; cnt++) {
        A278439_c[i] = cnt;
        int add = cnt ? (cnt < 10 ? 2 : 3) : 0;
        A278439_dfs(i + 1, s - cnt, L + add);
    }
    A278439_c[i] = 0;
}

static void A278439_init(void)
{
    if (A278439_done) return;
    A278439_done = true;
    for (uint64_t k = 1; k < 1000000ULL; k++) {
        int cnt[10] = {0};
        uint64_t t = k;
        while (t > 0) { cnt[t % 10]++; t /= 10; }
        uint64_t D = 0;
        for (int i = 0; i < 10; i++) {
            if (cnt[i] == 0) continue;
            D = D * 100 + (uint64_t)(cnt[i] * 10 + i);
        }
        if (D % k == 0) A278439_add((A278439_u128)k);
    }
    for (int d = 7; d <= 30; d++) {
        A278439_d = d;
        for (int i = 0; i < 10; i++) A278439_c[i] = 0;
        A278439_dfs(0, d, 0);
    }
    for (int i = 1; i < A278439_cnt; i++) {
        A278439_u128 v = A278439_res[i];
        int j = i - 1;
        while (j >= 0 && A278439_res[j] > v) {
            A278439_res[j + 1] = A278439_res[j];
            j--;
        }
        A278439_res[j + 1] = v;
    }
}

seq_t a(idx_t n)
{
    A278439_init();
    if (n < 1 || n > (idx_t)A278439_cnt) return (seq_t)0;
    A278439_u128 v = A278439_res[n - 1];
    seq_t r = (seq_t)(uint64_t)(v >> 64);
    r = r * ((seq_t)1 << 32);
    r = r * ((seq_t)1 << 32);
    r = r + (seq_t)(uint64_t)v;
    return r;
}
