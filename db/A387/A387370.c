/*
 * A387370  Smallest k for which A386310(k) = n, or -1 if no such k exists.
 * https://oeis.org/A387370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with A386310(k) = n, found by a pruned search
 *            over prime signatures k = 2^e0 * prod p^e. A divisor d counts
 *            only if every prime of k (2 only when e0 >= 2) divides d, so
 *            A386310(k) = T - F, where T = f(e0) * prod e (f(0)=1, f(1)=2,
 *            f(e)=e) and F counts the failing small d. When r...
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 92, u64 n <= 121, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 119 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define A387370_NB 512
#define A387370_MAXP 1000
#define A387370_STK 256

static int A387370_PR[200];
static double A387370_LP[200];
static int A387370_NP = 0;

static int A387370_n;
static int A387370_lim;
static double A387370_cap;
static double A387370_bestlog;
static unsigned char A387370_best[A387370_NB];
static int A387370_have;

static int A387370_m;
static int A387370_p[A387370_STK];
static int A387370_e[A387370_STK];
static int A387370_b[A387370_STK];

static void A387370_init(void) {
    static unsigned char comp[A387370_MAXP + 1];
    int i, j;
    if (A387370_NP) return;
    for (i = 2; i <= A387370_MAXP; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= A387370_MAXP; j += i) comp[j] = 1;
        if (i > 2) {
            A387370_PR[A387370_NP] = i;
            A387370_LP[A387370_NP] = log((double)i);
            A387370_NP++;
        }
    }
}

static int A387370_rec(int i, uint64_t d) {
    int total = 0, start = 1, bb, j;
    uint64_t cap = (uint64_t)1 << 30;
    uint64_t dd = d;
    if (i == A387370_m) {
        for (j = 0; j < A387370_m; j++) {
            uint64_t v = d * (uint64_t)A387370_b[j];
            if (A387370_p[j] == 2) v += 1;
            if (v < (uint64_t)A387370_e[j]) return 0;
        }
        return 1;
    }
    if (A387370_p[i] == 2 && A387370_e[i] <= 1) start = 0;
    if (start == 1) {
        dd = d * (uint64_t)A387370_p[i];
        if (dd > cap) dd = cap;
    }
    for (bb = start; bb <= A387370_e[i]; bb++) {
        A387370_b[i] = bb;
        total += A387370_rec(i + 1, dd);
        dd = dd * (uint64_t)A387370_p[i];
        if (dd > cap) dd = cap;
    }
    return total;
}

static void A387370_consider(double cost) {
    unsigned char k[A387370_NB];
    int i, j, t;
    for (i = 0; i < A387370_NB; i++) k[i] = 0;
    k[0] = 1;
    for (j = 0; j < A387370_m; j++) {
        for (t = 0; t < A387370_e[j]; t++) {
            unsigned int carry = 0;
            for (i = 0; i < A387370_NB; i++) {
                unsigned int v = (unsigned int)k[i] * (unsigned int)A387370_p[j] + carry;
                k[i] = (unsigned char)(v & 255u);
                carry = v >> 8;
            }
        }
    }
    if (A387370_have) {
        int cmp = 0;
        for (i = A387370_NB - 1; i >= 0; i--) {
            if (k[i] != A387370_best[i]) { cmp = k[i] < A387370_best[i] ? -1 : 1; break; }
        }
        if (cmp >= 0) return;
    }
    for (i = 0; i < A387370_NB; i++) A387370_best[i] = k[i];
    A387370_have = 1;
    A387370_bestlog = cost;
}

static double A387370_bound(void) {
    double b = A387370_cap;
    if (A387370_have && A387370_bestlog < b) b = A387370_bestlog;
    return b + 1e-9;
}

static double A387370_lb(double lp, int Pc) {
    int n = A387370_n;
    double lb = lp;
    if (Pc < n) {
        int R = (n + Pc - 1) / Pc;
        lb = lp * 1.89 * (log((double)R) / log(2.0));
        if (lb < lp) lb = lp;
    }
    return lb;
}

/* rad = product of primes every counted divisor must contain (capped at lim).
   rad >= lim  =>  no failures possible: count == Pc exactly. */
static void A387370_dfs(int j, int Pc, double cost, int rad, int maxE) {
    int n = A387370_n, lim = A387370_lim;
    int k, e;
    if (rad >= lim) {
        int R;
        if (Pc == n) { A387370_consider(cost); return; }
        if (Pc > n || n % Pc != 0) return;
        R = n / Pc;
        /* no failures: use the next prime, exponents non-increasing, dividing R */
        if (j >= A387370_NP || A387370_m >= A387370_STK) return;
        {
            double lp = A387370_LP[j];
            if (cost + A387370_lb(lp, Pc) > A387370_bound()) return;
            for (e = 2; e <= R && e <= maxE; e++) {
                double c2;
                if (R % e) continue;
                c2 = cost + e * lp;
                if (c2 > A387370_bound()) break;
                A387370_p[A387370_m] = A387370_PR[j];
                A387370_e[A387370_m] = e;
                A387370_m++;
                A387370_dfs(j + 1, Pc * e, c2, lim, e);
                A387370_m--;
            }
        }
        return;
    }
    if (Pc >= n && Pc <= lim) {
        if (A387370_rec(0, 1) == n) A387370_consider(cost);
    }
    for (k = j; k < A387370_NP; k++) {
        double lp = A387370_LP[k];
        long long nr;
        if (cost + A387370_lb(lp, Pc) > A387370_bound()) break;
        if (A387370_m >= A387370_STK) break;
        nr = (long long)rad * A387370_PR[k];
        if (nr > lim) nr = lim;
        for (e = 1; (long long)Pc * e <= lim; e++) {
            double c2 = cost + e * lp;
            if (c2 > A387370_bound()) break;
            A387370_p[A387370_m] = A387370_PR[k];
            A387370_e[A387370_m] = e;
            A387370_m++;
            A387370_dfs(k + 1, Pc * e, c2, (int)nr, lim);
            A387370_m--;
        }
    }
}

seq_t a(idx_t n) {
    seq_t r = (seq_t)0;
    int i;
    double l2 = log(2.0);
    if (n == 0) return (seq_t)0;
    A387370_init();
    A387370_n = (int)n;
    A387370_lim = 2 * (int)n + 64;
    A387370_have = 0;
    A387370_cap = log((double)n) + 1.0;
    while (!A387370_have) {
        int e2;
        for (e2 = 0; ; e2++) {
            int f2 = e2 == 0 ? 1 : (e2 == 1 ? 2 : e2);
            double cost = e2 * l2;
            if (f2 > A387370_lim) break;
            if (cost > A387370_bound()) break;
            A387370_m = 0;
            if (e2 > 0) { A387370_p[0] = 2; A387370_e[0] = e2; A387370_m = 1; }
            A387370_dfs(0, f2, cost, e2 >= 2 ? 2 : 1, A387370_lim);
        }
        if (!A387370_have) A387370_cap *= 1.3;
    }
    for (i = A387370_NB - 1; i >= 0; i--) {
        r = r * 256 + (seq_t)A387370_best[i];
    }
    return r;
}
