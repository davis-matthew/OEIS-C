/*
 * A308283  G.f. A(x) satisfies: A(x) = (1/(1 - x)) * A(x^3)*A(x^5)*A(x^7)*
 *          ... *A(x^(2*k-1))* ...
 * https://oeis.org/A308283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = (1/(1-x)) * Product_{k>=2} A(x^(2k-1)), equivalently G.f.
 *            Product_{k>=1} 1/(1-x^(2k-1))^A074206(2k-1). Fixed-width seq_t:
 *            use the functional equation with no division, so the result is
 *            exact modulo 2^w. a(n) = Sum_{m<=n} [x^m] Product_{odd j>=3}
 *            A(x^j), and the product only needs a(i) for...
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 180, u64 n <= 529,
 *            u128 n <= 1630, big any n
 * Validated: standard, pass, 72 terms through n = 9086 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A308283_cache = NULL;
static idx_t A308283_len = 0;

static bool A308283_is_big(void)
{
    seq_t t = (seq_t)1;
    for (int i = 0; i < 200; ++i)
        t = t * (seq_t)2;
    return t != (seq_t)0;
}

/* Division-free computation of a(0..N); the values wrap modulo 2^w */
static seq_t *A308283_fixed(idx_t N)
{
    seq_t *res = mem_new(seq_t, (size_t)N + 1);
    if (N < 3) {
        for (idx_t i = 0; i <= N; ++i)
            res[i] = (seq_t)1;
        return res;
    }
    seq_t *sub = A308283_fixed(N / 3);
    seq_t *r = mem_new(seq_t, (size_t)N + 1);
    r[0] = (seq_t)1;
    for (idx_t j = 3; j <= N; j += 2) {
        for (idx_t m = N; m >= j; --m) {
            seq_t s = (seq_t)0;
            idx_t lim = m / j;
            for (idx_t i = 1; i <= lim; ++i)
                s += sub[i] * r[m - i * j];
            r[m] += s;
        }
    }
    seq_t acc = (seq_t)0;
    for (idx_t i = 0; i <= N; ++i) {
        acc += r[i];
        res[i] = acc;
    }
    mem_free(r);
    mem_free(sub);
    return res;
}

/* Exact Euler-transform recurrence; intended for arbitrary-precision seq_t */
static seq_t *A308283_big(idx_t N)
{
    seq_t *b = mem_new(seq_t, (size_t)N + 1);
    seq_t *c = mem_new(seq_t, (size_t)N + 1);
    b[1] = (seq_t)1;
    for (idx_t d = 1; d <= N; ++d)
        for (idx_t j = 2 * d; j <= N; j += d)
            b[j] += b[d];
    for (idx_t d = 1; d <= N; d += 2) {
        seq_t t = (seq_t)d * b[d];
        for (idx_t j = d; j <= N; j += d)
            c[j] += t;
    }
    seq_t *r = mem_new(seq_t, (size_t)N + 1);
    r[0] = (seq_t)1;
    for (idx_t m = 1; m <= N; ++m) {
        seq_t s = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
            s += c[k] * r[m - k];
        r[m] = s / (seq_t)m;
    }
    mem_free(b);
    mem_free(c);
    return r;
}

seq_t a(idx_t n)
{
    if (n < A308283_len)
        return A308283_cache[n];

    idx_t N = n;
    if (N < 2 * A308283_len)
        N = 2 * A308283_len;
    if (N < 64)
        N = 64;

    seq_t *r = A308283_is_big() ? A308283_big(N) : A308283_fixed(N);

    if (A308283_cache)
        mem_free(A308283_cache);
    A308283_cache = r;
    A308283_len = N + 1;
    return r[n];
}
