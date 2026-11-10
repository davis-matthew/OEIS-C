/*
 * A345955  Number of isomorphism classes of indecomposable Fano Bott
 *          manifolds of complex dimension n.
 * https://oeis.org/A345955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] (F(x^2)+F(x)^2)/2, F = g.f. of A003080 (F(0)=1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-10)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

static seq_t *A003080_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *A003080_memo = 0;
static idx_t A003080_len = 0;

static void A003080_build(idx_t N) {
    seq_t *f = A003080_alloc_zero(N + 1);
    seq_t *av = A003080_alloc_zero(N + 1);
    seq_t *B = A003080_alloc_zero(N + 1);
    idx_t k, m, i, j;
    f[0] = 1;
    for (k = 1; k <= N; k++) {
        idx_t jj = k - 1, J;
        seq_t c = 0;
        av[k - 1] = f[k - 1];
        for (i = 0; 2 * i < jj; i++) c += av[i] * av[jj - i];
        if ((jj & 1) == 0) {
            seq_t h = av[jj / 2];
            if (h % 2 == 0) c += (h / 2) * (h + 1);
            else c += h * ((h + 1) / 2);
        }
        J = N / k;
        B[0] = 1;
        for (i = 1; i <= J; i++) {
            seq_t t = c + (seq_t)(i - 1);
            unsigned long long r = (unsigned long long)(B[i - 1] % (seq_t)i);
            unsigned long long g = gcd_u64(r, (unsigned long long)i);
            if (g == 0) g = (unsigned long long)i;
            B[i] = (B[i - 1] / (seq_t)g) * (t / (seq_t)((unsigned long long)i / g));
        }
        for (m = N; m >= k; m--) {
            seq_t s = f[m];
            for (j = 1; j * k <= m; j++) s += B[j] * f[m - j * k];
            f[m] = s;
        }
    }
    av[N] = f[N];
    mem_free(f);
    mem_free(B);
    if (A003080_memo) mem_free(A003080_memo);
    A003080_memo = av;
    A003080_len = N + 1;
}

static seq_t A003080_a(idx_t n) {
    if (n >= A003080_len) {
        idx_t N = n;
        if (N < 2 * A003080_len) N = 2 * A003080_len;
        if (N < 16) N = 16;
        A003080_build(N);
    }
    return A003080_memo[n];
}

seq_t a(idx_t n) {
    idx_t m = n - 1, i;
    seq_t s = 0;
    seq_t *F = mem_new(seq_t, (size_t)(m + 1));
    for (i = 0; i <= m; i++) F[i] = A003080_a(i);
    for (i = 0; 2 * i < m; i++) s += F[i] * F[m - i];
    if ((m & 1) == 0) {
        seq_t h = F[m / 2];
        if (h % 2 == 0) s += (h / 2) * (h + 1);
        else s += h * ((h + 1) / 2);
    }
    mem_free(F);
    return s;
}
