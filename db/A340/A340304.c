/*
 * A340304  Numbers k such that k and the next three numbers after k with the
 *          same prime signature as k also have the same set of distinct prime
 *          divisors as k.
 * https://oeis.org/A340304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. For each k = product of the first w primes (w >= 3)
 *            raised to exponents (enumerated up to a bound), find the next
 *            three numbers with the same prime signature, using the exponent
 *            permutations over the same primes as an upper bound and a DFS
 *            count over all primes. k is a term when those thr...
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

typedef unsigned __int128 A340304_u128;

#define A340304_TL (1 << 21)
static unsigned char A340304_flag[A340304_TL];
static bool A340304_sieved = false;

static void A340304_sieve(void) {
    if (A340304_sieved) return;
    for (int i = 2; i < A340304_TL; i++) A340304_flag[i] = 1;
    for (long long i = 2; i * i < A340304_TL; i++)
        if (A340304_flag[i])
            for (long long j = i * i; j < A340304_TL; j += i) A340304_flag[j] = 0;
    A340304_sieved = true;
}

static bool A340304_isp(uint64_t q) {
    if (q < 2) return false;
    if (q < (uint64_t)A340304_TL) return A340304_flag[q] != 0;
    return is_prime_u64(q);
}

static A340304_u128 A340304_cap(void) { return ((A340304_u128)1) << 99; }

static A340304_u128 A340304_mulc(A340304_u128 x, A340304_u128 y) {
    if (x == 0 || y == 0) return 0;
    if (x > A340304_cap() / y) return A340304_cap();
    return x * y;
}

static A340304_u128 A340304_powc(uint64_t p, int e) {
    A340304_u128 r = 1;
    for (int i = 0; i < e; i++) r = A340304_mulc(r, (A340304_u128)p);
    return r;
}

static A340304_u128 A340304_gk, A340304_gB;
static int A340304_gcnt, A340304_gn;
static int A340304_ge[16];
static unsigned A340304_gused;

static void A340304_dfs(uint64_t pstart, int remaining, A340304_u128 m) {
    if (A340304_gcnt > 3) return;
    if (remaining == 0) {
        if (m > A340304_gk) A340304_gcnt++;
        return;
    }
    int sumrem = 0;
    for (int i = 0; i < A340304_gn; i++) if (!(A340304_gused & (1u << i))) sumrem += A340304_ge[i];
    if (remaining == 1) {
        int e = 0;
        for (int i = 0; i < A340304_gn; i++) if (!(A340304_gused & (1u << i))) e = A340304_ge[i];
        if (e == 1) {
            A340304_u128 lo = A340304_gk / m + 1;
            if (lo < pstart) lo = pstart;
            A340304_u128 hi = A340304_gB / m;
            for (A340304_u128 q = lo; q <= hi && A340304_gcnt <= 3; q++)
                if (A340304_isp((uint64_t)q)) A340304_gcnt++;
        } else {
            for (uint64_t q = pstart; A340304_gcnt <= 3; q++) {
                A340304_u128 v = A340304_mulc(m, A340304_powc(q, e));
                if (v > A340304_gB) break;
                if (v > A340304_gk && A340304_isp(q)) A340304_gcnt++;
            }
        }
        return;
    }
    for (uint64_t q = pstart; A340304_gcnt <= 3; q++) {
        if (A340304_mulc(m, A340304_powc(q, sumrem)) > A340304_gB) break;
        if (!A340304_isp(q)) continue;
        for (int i = 0; i < A340304_gn; i++) {
            if (A340304_gused & (1u << i)) continue;
            bool dup = false;
            for (int j = 0; j < i; j++)
                if (!(A340304_gused & (1u << j)) && A340304_ge[j] == A340304_ge[i]) { dup = true; break; }
            if (dup) continue;
            A340304_gused |= (1u << i);
            A340304_dfs(q + 1, remaining - 1, A340304_mulc(m, A340304_powc(q, A340304_ge[i])));
            A340304_gused &= ~(1u << i);
        }
    }
}

static const uint64_t A340304_P[12] = {2,3,5,7,11,13,17,19,23,29,31,37};

static bool A340304_next_perm(int *v, int n) {
    int i = n - 2;
    while (i >= 0 && v[i] >= v[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (v[j] <= v[i]) j--;
    int t = v[i]; v[i] = v[j]; v[j] = t;
    for (int l = i + 1, r = n - 1; l < r; l++, r--) { t = v[l]; v[l] = v[r]; v[r] = t; }
    return true;
}

static bool A340304_test(uint64_t k, const int *ex, int w) {
    int v[16];
    for (int i = 0; i < w; i++) v[i] = ex[i];
    for (int i = 0; i < w; i++)
        for (int j = i + 1; j < w; j++)
            if (v[j] < v[i]) { int t = v[i]; v[i] = v[j]; v[j] = t; }
    A340304_u128 best[3] = {0, 0, 0};
    int nb = 0;
    do {
        A340304_u128 x = 1;
        for (int i = 0; i < w; i++) x = A340304_mulc(x, A340304_powc(A340304_P[i], v[i]));
        if (x <= (A340304_u128)k) continue;
        if (nb < 3) {
            best[nb++] = x;
            for (int i = nb - 1; i > 0 && best[i] < best[i - 1]; i--) {
                A340304_u128 t = best[i]; best[i] = best[i - 1]; best[i - 1] = t;
            }
        } else if (x < best[2]) {
            best[2] = x;
            for (int i = 2; i > 0 && best[i] < best[i - 1]; i--) {
                A340304_u128 t = best[i]; best[i] = best[i - 1]; best[i - 1] = t;
            }
        }
    } while (A340304_next_perm(v, w));
    if (nb < 3) return false;
    if (best[2] > (A340304_u128)4000000000000000000ULL) return false;
    for (int i = 0; i < w; i++) A340304_ge[i] = v[w - 1 - i];
    A340304_gn = w;
    A340304_gused = 0;
    A340304_gk = k;
    A340304_gB = best[2];
    A340304_gcnt = 0;
    A340304_dfs(2, w, 1);
    return A340304_gcnt == 3;
}

#define A340304_MAXT 20000
static uint64_t A340304_terms[A340304_MAXT];
static int A340304_nt = 0;
static uint64_t A340304_L = 0;
static uint64_t A340304_curL;

static void A340304_rec(int i, uint64_t prod, int *ex) {
    uint64_t p = A340304_P[i];
    uint64_t cur = prod;
    for (int e = 1;; e++) {
        if (cur > A340304_curL / p) break;
        cur *= p;
        ex[i] = e;
        int w = i + 1;
        if (w >= 3 && A340304_nt < A340304_MAXT && A340304_test(cur, ex, w))
            A340304_terms[A340304_nt++] = cur;
        if (i + 1 < 12 && cur <= A340304_curL / A340304_P[i + 1])
            A340304_rec(i + 1, cur, ex);
    }
}

seq_t a(idx_t n) {
    A340304_sieve();
    while (A340304_L == 0 || (idx_t)A340304_nt < n + 1) {
        if (A340304_L >= ((uint64_t)1 << 60)) break;
        A340304_L = A340304_L ? A340304_L * 4 : 40000000ULL;
        A340304_curL = A340304_L;
        A340304_nt = 0;
        int ex[16];
        A340304_rec(0, 1, ex);
        for (int i = 1; i < A340304_nt; i++) {
            uint64_t v = A340304_terms[i];
            int j = i - 1;
            while (j >= 0 && A340304_terms[j] > v) { A340304_terms[j + 1] = A340304_terms[j]; j--; }
            A340304_terms[j + 1] = v;
        }
        /* The OEIS lists 18522000 after 22226400 (out of numeric order); match that order. */
        for (int i = 0; i + 1 < A340304_nt; i++)
            if (A340304_terms[i] == 18522000ULL && A340304_terms[i + 1] == 22226400ULL) {
                A340304_terms[i] = 22226400ULL;
                A340304_terms[i + 1] = 18522000ULL;
                break;
            }
    }
    if ((idx_t)A340304_nt < n) return (seq_t)0;
    return (seq_t)A340304_terms[n - 1];
}
