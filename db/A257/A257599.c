/*
 * A257599  Cusp numbers of terms of A091401.
 * https://oeis.org/A257599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of cusps of Gamma_0(m), m = A091401(n), i.e.
 *            sum_{d|m} phi(gcd(d,m/d)); search over m with genus 0 (12 + mu -
 *            3*e2 - 4*e3 - 6*cusps = 0)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static void a257599_factor(long long n, long long *pr, int *ex, int *cnt)
{
    int c = 0;
    for (long long f = 2; f * f <= n; ++f) {
        if (n % f == 0) {
            int e = 0;
            while (n % f == 0) { n /= f; ++e; }
            pr[c] = f; ex[c] = e; ++c;
        }
    }
    if (n > 1) { pr[c] = n; ex[c] = 1; ++c; }
    *cnt = c;
}

static long long a257599_cusps(long long m)
{
    long long pr[32];
    int ex[32];
    int cnt;
    long long total = 1;
    a257599_factor(m, pr, ex, &cnt);
    for (int i = 0; i < cnt; ++i) {
        long long p = pr[i];
        int e = ex[i];
        long long pw[70];
        long long cp = 0;
        pw[0] = 1;
        for (int j = 1; j <= e; ++j) pw[j] = pw[j - 1] * p;
        for (int k = 0; k <= e; ++k) {
            int mm = (k < e - k) ? k : (e - k);
            if (mm == 0) cp += 1;
            else cp += pw[mm] - pw[mm - 1];
        }
        total *= cp;
    }
    return total;
}

static bool a257599_genus_zero(long long m)
{
    long long pr[32];
    int ex[32];
    int cnt;
    if (m == 1) return true;
    a257599_factor(m, pr, ex, &cnt);
    long long mu = m;
    long long e2 = 1, e3 = 1;
    for (int i = 0; i < cnt; ++i) {
        mu = mu / pr[i] * (pr[i] + 1);
        if (e2) {
            if (pr[i] == 2) { if (ex[i] != 1) e2 = 0; }
            else if (pr[i] % 4 == 1) e2 *= 2;
            else e2 = 0;
        }
        if (e3) {
            if (pr[i] == 3) { if (ex[i] != 1) e3 = 0; }
            else if (pr[i] % 3 == 1) e3 *= 2;
            else e3 = 0;
        }
    }
    long long c = a257599_cusps(m);
    return 12 + mu - 3 * e2 - 4 * e3 - 6 * c == 0;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 15) return (seq_t)0;
    idx_t found = 0;
    for (long long m = 1; m <= 25; ++m) {
        if (a257599_genus_zero(m)) {
            ++found;
            if (found == n) return (seq_t)a257599_cusps(m);
        }
    }
    return (seq_t)0;
}
