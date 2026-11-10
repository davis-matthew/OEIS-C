/*
 * A353736  Number of n-digit terms in A353007.
 * https://oeis.org/A353736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = T(n) - U(n-1) for n>=2 (a(1)=5), where T(m) = m![x^m]
 *            cosh(x)^5 (1+sinh(x))^5 and U(m) = m![x^m] cosh(x)^6
 *            (1+sinh(x))^4 (the second removes strings with a leading 0);
 *            computed by a multinomial DP for small n and by an
 *            exponential-sum closed form for large n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 993 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count length-n digit strings: first a digits have even counts, remaining b digits have count 0 or odd. */
static seq_t A353736_count_dp(int n, int a, int b)
{
    int w = n + 1;
    (void)b;
    seq_t *C = mem_new(seq_t, (size_t)w * (size_t)w);
    seq_t *f = mem_new(seq_t, (size_t)w);
    seq_t *g = mem_new(seq_t, (size_t)w);
    for (int m = 0; m <= n; m++) {
        C[m * w + 0] = (seq_t)1;
        for (int k = 1; k <= m; k++) {
            seq_t v = C[(m - 1) * w + (k - 1)];
            if (k <= m - 1) v = v + C[(m - 1) * w + k];
            C[m * w + k] = v;
        }
    }
    f[0] = (seq_t)1;
    for (int d = 0; d < 10; d++) {
        bool evenType = d < a;
        for (int m = 0; m <= n; m++) {
            seq_t s = (seq_t)0;
            for (int k = 0; k <= m; k++) {
                bool ok;
                if (evenType) ok = (k % 2 == 0);
                else ok = (k == 0) || (k % 2 == 1);
                if (!ok) continue;
                s = s + C[m * w + k] * f[m - k];
            }
            g[m] = s;
        }
        for (int m = 0; m <= n; m++) f[m] = g[m];
    }
    seq_t r = f[n];
    mem_free(C);
    mem_free(f);
    mem_free(g);
    return r;
}

static seq_t A353736_pw(seq_t base, idx_t e)
{
    seq_t r = (seq_t)1;
    while (e > 0) {
        if (e & 1) r = r * base;
        base = base * base;
        e >>= 1;
    }
    return r;
}

/* Closed form: 2^-10 * sum_k c_k k^n, with sum_j c_j u^j = (u^2+1)^a (u^2+2u-1)^b, k=j-10. */
static seq_t A353736_count_exp(idx_t n, int a, int b)
{
    long long P[32];
    long long T[32];
    int deg = 0;
    P[0] = 1;
    for (int i = 0; i < 32; i++) { P[i] = 0; T[i] = 0; }
    P[0] = 1;
    for (int t = 0; t < a + b; t++) {
        long long q0, q1, q2;
        if (t < a) { q0 = 1; q1 = 0; q2 = 1; }
        else { q0 = -1; q1 = 2; q2 = 1; }
        for (int i = 0; i < 32; i++) T[i] = 0;
        for (int i = 0; i <= deg; i++) {
            T[i] += P[i] * q0;
            T[i + 1] += P[i] * q1;
            T[i + 2] += P[i] * q2;
        }
        deg += 2;
        for (int i = 0; i < 32; i++) P[i] = T[i];
    }
    seq_t pos = (seq_t)0, neg = (seq_t)0;
    for (int j = 0; j <= deg; j++) {
        long long c = P[j];
        int k = j - (a + b);
        if (c == 0 || k == 0) continue;
        bool negative = (c < 0);
        if (c < 0) c = -c;
        int ak = k < 0 ? -k : k;
        if (k < 0 && (n & 1)) negative = !negative;
        seq_t term = (seq_t)c * A353736_pw((seq_t)ak, n);
        if (negative) neg = neg + term;
        else pos = pos + term;
    }
    seq_t num = pos - neg;
    return num / (seq_t)1024;
}

static seq_t A353736_count(idx_t n, int a, int b)
{
    if (n <= 64) return A353736_count_dp((int)n, a, b);
    return A353736_count_exp(n, a, b);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;
    seq_t t = A353736_count(n, 5, 5);
    seq_t u = A353736_count(n - 1, 6, 4);
    return t - u;
}
