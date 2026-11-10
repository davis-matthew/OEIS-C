/*
 * A324194  A000120-deficiency of n permuted by A163511: a(n) =
 *          A192895(A163511(n)).
 * https://oeis.org/A324194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A192895(A163511(n)); A163511(n) = p(r)*Prod_{i=1..r}
 *            p(i)^g_i, where g_i = zeros between the (i-1)-th and i-th 1-bit
 *            of n (counting from the low end) and r = A000120(n). A192895(m)
 *            = Sum_{d|m, d<m} A000120(d) - A000120(m). Divisors are
 *            enumerated from the known factorization
 * Range:     i8 n <= 32, i16 n <= 916, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9227 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A324194_pop(seq_t x)
{
    int c = 0;
    while (x != 0) {
        c += (int)(x & 1);
        x >>= 1;
    }
    return c;
}

static void A324194_dfs(const uint32_t *pr, const int *ex, int k, int idx,
                        seq_t cur, long long *total)
{
    if (idx == k) {
        *total += A324194_pop(cur);
        return;
    }
    for (int j = 0; j <= ex[idx]; ++j) {
        A324194_dfs(pr, ex, k, idx + 1, cur, total);
        cur *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;

    uint32_t primes[64];
    int np = 0;
    for (uint32_t c = 2; np < 64; ++c) {
        int ok = 1;
        for (int i = 0; i < np && primes[i] * primes[i] <= c; ++i) {
            if (c % primes[i] == 0) { ok = 0; break; }
        }
        if (ok) primes[np++] = c;
    }

    int e[64];
    int r = 0;
    int gap = 0;
    idx_t t = n;
    while (t) {
        if (t & 1) {
            e[r++] = gap;
            gap = 0;
        } else {
            ++gap;
        }
        t >>= 1;
    }
    e[r - 1] += 1;

    uint32_t pr[64];
    int ex[64];
    int k = 0;
    for (int i = 0; i < r; ++i) {
        if (e[i] > 0) {
            pr[k] = primes[i];
            ex[k] = e[i];
            ++k;
        }
    }

    long long total = 0;
    A324194_dfs(pr, ex, k, 0, (seq_t)1, &total);

    /* m = A163511(n) */
    seq_t m = 1;
    for (int i = 0; i < k; ++i)
        for (int j = 0; j < ex[i]; ++j)
            m *= (seq_t)pr[i];

    return (seq_t)(total - 2LL * A324194_pop(m));
}
