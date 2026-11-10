/*
 * A273445  a(n) is the number of solutions of the equation n = A001617(k).
 * https://oeis.org/A273445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = card {k >= 1 : genus of X_0(k) = n}; computed by sieving
 *            the genus g(k) = 1 + psi(k)/12 - nu2/4 - nu3/3 - cusps/2 over
 *            all k up to a bound that guarantees g(k) > n beyond it (k <=
 *            (6+sqrt(12n+38))^2), then counting
 * Range:     u8 n <= 100001, u16 n <= 100001, u32 n <= 100001,
 *            u64 n <= 100001, u128 n <= 100001, big any n
 * Validated: standard, pass, 88 terms through n = 9340 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static long long A273445_pp_cusps(long long p, int e)
{
    long long pw[64];
    long long s = 0;
    int j, m;
    pw[0] = 1;
    for (j = 1; j <= e; j++) pw[j] = pw[j - 1] * p;
    for (j = 0; j <= e; j++) {
        m = (j < e - j) ? j : (e - j);
        if (m == 0) s += 1;
        else s += pw[m] - pw[m - 1];
    }
    return s;
}

seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n;
    unsigned long long t = 12ULL * N + 38ULL;
    unsigned long long r = 0;
    while ((r + 1) * (r + 1) <= t) r++;
    unsigned long long L = (6ULL + r + 1ULL) * (6ULL + r + 1ULL) + 10ULL;

    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    if (!spf) return (seq_t)0;
    for (unsigned long long i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (unsigned long long j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    unsigned long long count = 0;
    for (unsigned long long k = 1; k <= L; k++) {
        unsigned long long m = k;
        long long psi = (long long)k;
        long long nu2 = (k % 4 == 0) ? 0 : 1;
        long long nu3 = (k % 9 == 0) ? 0 : 1;
        long long cusps = 1;
        while (m > 1) {
            unsigned long long p = spf[m];
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            psi = psi / (long long)p * (long long)(p + 1);
            if (p != 2) nu2 *= (p % 4 == 1) ? 2 : 0;
            if (p != 3) nu3 *= (p % 3 == 1) ? 2 : 0;
            cusps *= A273445_pp_cusps((long long)p, e);
        }
        long long num = 12 + psi - 3 * nu2 - 4 * nu3 - 6 * cusps;
        long long g = num / 12;
        if (g >= 0 && (unsigned long long)g == N) count++;
    }
    free(spf);
    return (seq_t)count;
}
