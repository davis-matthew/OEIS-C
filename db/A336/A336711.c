/*
 * A336711  Irregular triangle where the n-th row lists the positive integers
 *          k such that phi(k) = A002034(k^n).
 * https://oeis.org/A336711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n lists k with phi(k) = A002034(k^n); found by a bounded
 *            search over k with a sieve, using the bounds n*max(e*(p-1))+1 <=
 *            S(k^n) <= n*max(e*p). Row n is the n-th row of the triangle
 * Range:     u8 n <= 25, u16 n <= 315, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 76 terms through n = 877 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned long long K336711_ull;

/* smallest M with p^m | M! */
static K336711_ull K336711_Spp(K336711_ull p, K336711_ull m) {
    K336711_ull lo = 0, hi = p * m;
    while (lo < hi) {
        K336711_ull mid = lo + (hi - lo) / 2;
        K336711_ull v = 0, t = mid;
        while (t) { t /= p; v += t; }
        if (v >= m) hi = mid; else lo = mid + 1;
    }
    return lo;
}

/* S(k^n) */
static K336711_ull K336711_S(K336711_ull k, K336711_ull n, const uint32_t *spf) {
    K336711_ull best = 0;
    while (k > 1) {
        K336711_ull p = spf[k], e = 0;
        while (k % p == 0) { k /= p; e++; }
        K336711_ull s = K336711_Spp(p, e * n);
        if (s > best) best = s;
    }
    return best;
}

/* computes phi and the candidate range [lo,hi] of n for k >= 2 */
static void K336711_info(K336711_ull k, const uint32_t *spf, K336711_ull *phi,
                         K336711_ull *lo, K336711_ull *hi) {
    K336711_ull m = k, ph = 1, mep = 0, mep1 = 0;
    while (m > 1) {
        K336711_ull p = spf[m], e = 0, pe = 1;
        while (m % p == 0) { m /= p; e++; pe *= p; }
        ph *= (pe / p) * (p - 1);
        if (e * p > mep) mep = e * p;
        if (e * (p - 1) > mep1) mep1 = e * (p - 1);
    }
    *phi = ph;
    *lo = (ph + mep - 1) / mep;
    if (*lo < 1) *lo = 1;
    *hi = (ph - 1) / mep1;
}

seq_t a(idx_t idx) {
    K336711_ull N = 8;
    K336711_ull target = (K336711_ull)idx;
    uint32_t *spf = 0;
    K336711_ull B = 0;
    K336711_ull *cnt = 0;
    K336711_ull r = 0, j = 0;
    for (;;) {
        B = 24 * N * N + 100;
        spf = (uint32_t *)malloc((B + 1) * sizeof(uint32_t));
        cnt = (K336711_ull *)calloc(N + 2, sizeof(K336711_ull));
        for (K336711_ull i = 0; i <= B; i++) spf[i] = 0;
        for (K336711_ull i = 2; i <= B; i++) {
            if (spf[i] == 0) {
                for (K336711_ull q = i; q <= B; q += i)
                    if (spf[q] == 0) spf[q] = (uint32_t)i;
            }
        }
        for (K336711_ull n = 1; n <= N; n++) cnt[n] = 1;
        for (K336711_ull k = 2; k <= B; k++) {
            K336711_ull phi, lo, hi;
            K336711_info(k, spf, &phi, &lo, &hi);
            if (hi > N) hi = N;
            for (K336711_ull n = lo; n <= hi; n++)
                if (K336711_S(k, n, spf) == phi) cnt[n]++;
        }
        K336711_ull acc = 0;
        int found = 0;
        for (K336711_ull n = 1; n <= N; n++) {
            if (acc + cnt[n] >= target) {
                r = n; j = target - acc; found = 1; break;
            }
            acc += cnt[n];
        }
        free(cnt);
        if (found) break;
        free(spf);
        N *= 2;
    }
    if (j == 1) { free(spf); return (seq_t)1; }
    K336711_ull c = 1, ans = 1;
    for (K336711_ull k = 2; k <= B; k++) {
        K336711_ull phi, lo, hi;
        K336711_info(k, spf, &phi, &lo, &hi);
        if (r < lo || r > hi) continue;
        if (K336711_S(k, r, spf) == phi) {
            c++;
            if (c == j) { ans = k; break; }
        }
    }
    free(spf);
    return (seq_t)ans;
}
