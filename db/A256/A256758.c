/*
 * A256758  Position of first appearance of n in A256757.
 * https://oeis.org/A256758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1, a(1) = 2; for n >= 2, a(n) is the smallest odd q whose
 *            multiplicative order of 2, ord_q(2) = A007733(q), has depth n-1,
 *            where depth(1) = 0 and depth(m) = 1 + depth(A007733(m)). Search,
 *            working backward one level at a time: keep the smallest odd
 *            members of each level. For each member r a...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A256758_LV 40
#define A256758_T 64
#define A256758_CMAX 40
#define A256758_SMAX 31
#define A256758_LIM ((uint64_t)1 << 62)

typedef unsigned __int128 A256758_u128;

static uint64_t A256758_mm(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((A256758_u128)a * b % m);
}

static uint64_t A256758_pm(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A256758_mm(r, b, m);
        b = A256758_mm(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A256758_mr(uint64_t n) {
    static const uint64_t sp[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++)
        if (n % sp[i] == 0) return n == sp[i];
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = A256758_pm(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; r++) {
            x = A256758_mm(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static uint64_t A256758_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A256758_rho(uint64_t n) {
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (A256758_mm(x, x, n) + c) % n;
            y = (A256758_mm(y, y, n) + c) % n;
            y = (A256758_mm(y, y, n) + c) % n;
            d = A256758_gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static void A256758_addp(uint64_t *pf, int *np, uint64_t p) {
    for (int i = 0; i < *np; i++)
        if (pf[i] == p) return;
    pf[(*np)++] = p;
}

static void A256758_fac_rec(uint64_t n, uint64_t *pf, int *np) {
    if (n == 1) return;
    if (A256758_mr(n)) { A256758_addp(pf, np, n); return; }
    uint64_t d = A256758_rho(n);
    A256758_fac_rec(d, pf, np);
    A256758_fac_rec(n / d, pf, np);
}

static void A256758_factor(uint64_t n, uint64_t *pf, int *np) {
    *np = 0;
    for (uint64_t d = 2; d < 1000 && d * d <= n; d++) {
        if (n % d == 0) {
            A256758_addp(pf, np, d);
            while (n % d == 0) n /= d;
        }
    }
    A256758_fac_rec(n, pf, np);
}

static bool A256758_order_is(uint64_t m, uint64_t k, const uint64_t *pf, int np) {
    if (m < 3 || (m & 1) == 0) return false;
    if (A256758_pm(2, k, m) != 1) return false;
    for (int i = 0; i < np; i++)
        if (A256758_pm(2, k / pf[i], m) == 1) return false;
    return true;
}

static void A256758_add(uint64_t *lst, int *cnt, uint64_t v) {
    int i;
    for (i = 0; i < *cnt; i++)
        if (lst[i] == v) return;
    if (*cnt == A256758_T && v > lst[*cnt - 1]) return;
    i = *cnt;
    if (i == A256758_T) i--;
    else (*cnt)++;
    while (i > 0 && lst[i - 1] > v) {
        lst[i] = lst[i - 1];
        i--;
    }
    lst[i] = v;
}

static void A256758_mult(uint64_t q, uint64_t k, const uint64_t *kp, int nkp,
                         uint64_t *out, int *nc) {
    for (uint64_t s = 3; s <= A256758_SMAX; s += 2) {
        if (q > A256758_LIM / s) break;
        uint64_t q2 = s * q;
        if (*nc == A256758_T && q2 > out[A256758_T - 1]) break;
        if (A256758_order_is(q2, k, kp, nkp)) A256758_add(out, nc, q2);
    }
}

seq_t a(idx_t n) {
    static uint64_t S[A256758_LV][A256758_T];
    static int cnt[A256758_LV];
    static int built = 1;
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n >= A256758_LV) n = A256758_LV - 1;
    while (built < (int)n) {
        int j = built;
        int nc = 0;
        uint64_t *out = S[j + 1];
        uint64_t one = 1;
        const uint64_t *src = (j == 1) ? &one : S[j];
        int ns = (j == 1) ? 1 : cnt[j];
        for (int t = 0; t < ns; t++) {
            uint64_t r = src[t];
            uint64_t pf[64];
            int np = 0;
            if (r > 1) A256758_factor(r, pf, &np);
            int e0 = (r == 1) ? 1 : 0;
            for (int e = e0; e < 62; e++) {
                if (r > (A256758_LIM >> e)) break;
                uint64_t k = r << e;
                if (nc == A256758_T && k >= out[A256758_T - 1]) break;
                uint64_t kp[65];
                int nkp = 0;
                for (int i = 0; i < np; i++) kp[nkp++] = pf[i];
                if (e > 0) kp[nkp++] = 2;
                /* family A: q = c*k + 1 */
                for (uint64_t c = 1; c <= A256758_CMAX; c++) {
                    if (c > (A256758_LIM - 1) / k) break;
                    uint64_t q = c * k + 1;
                    if (nc == A256758_T && q > out[A256758_T - 1]) break;
                    if ((q & 1) && A256758_order_is(q, k, kp, nkp)) {
                        A256758_add(out, &nc, q);
                        A256758_mult(q, k, kp, nkp, out, &nc);
                    }
                }
                /* family B: q = p^(t+1), p^t || k */
                for (int i = 0; i < np; i++) {
                    uint64_t p = pf[i];
                    uint64_t pt = 1, tmp = k;
                    while (tmp % p == 0) { tmp /= p; pt *= p; }
                    if (pt > A256758_LIM / p) continue;
                    uint64_t q = pt * p;
                    if (nc == A256758_T && q > out[A256758_T - 1]) continue;
                    if (A256758_order_is(q, k, kp, nkp)) {
                        A256758_add(out, &nc, q);
                        A256758_mult(q, k, kp, nkp, out, &nc);
                    }
                }
            }
        }
        cnt[j + 1] = nc;
        built++;
    }
    if (cnt[n] == 0) return (seq_t)0;
    return (seq_t)S[n][0];
}
