/*
 * A307604  G.f. A(x) satisfies: A(x) = (1/(1 - x)) *
 *          A(x^2)^2*A(x^3)^3*A(x^4)^4* ... *A(x^k)^k* ...
 * https://oeis.org/A307604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. Product_{k>=1} 1/(1 - x^k)^(k*A074206(k)), i.e. the Euler *
 *            transform of A050369(k) = k*A074206(k). For multiword-safe
 *            exactness the * product is expanded factor by factor, using *
 *            1/(1-x^k)^e = Sum_{j>=0} binomial(e+j-1, j) x^(k*j); for
 *            arbitrary-precision * seq_t the equivalent recurrence...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 38, u64 n <= 91, u128 n <= 224,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 4862 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"

static seq_t *s_a = NULL;
static idx_t s_max = 0;
static int s_init = 0;

static int A307604_is_bignum(void)
{
    seq_t t = (seq_t)1;
    int i;
    for (i = 0; i < 140; ++i) t *= (seq_t)2;
    return t != (seq_t)0;
}

static void A307604_build(idx_t N)
{
    /* e[k] = k * A074206(k) = A050369(k) */
    seq_t *h = mem_new(seq_t, N + 1);   /* A074206 */
    seq_t *e = mem_new(seq_t, N + 1);
    seq_t *r = mem_new(seq_t, N + 1);
    idx_t i, k, m, j;
    h[1] = (seq_t)1;
    for (i = 1; i <= N; ++i) {
        for (m = 2 * i; m <= N; m += i) h[m] += h[i];
        e[i] = (seq_t)i * h[i];
    }

    if (A307604_is_bignum()) {
        seq_t *b = mem_new(seq_t, N + 1);
        for (i = 1; i <= N; ++i) {
            seq_t w = (seq_t)i * e[i];
            for (m = i; m <= N; m += i) b[m] += w;
        }
        r[0] = (seq_t)1;
        for (i = 1; i <= N; ++i) {
            seq_t sum = 0;
            for (k = 1; k <= i; ++k) sum += b[k] * r[i - k];
            r[i] = sum / (seq_t)i;
        }
        mem_free(b);
    } else {
        seq_t *c = mem_new(seq_t, N + 1);
        r[0] = (seq_t)1;
        for (k = 1; k <= N; ++k) {
            idx_t J = N / k;
            seq_t ek = e[k];
            c[0] = (seq_t)1;
            for (j = 1; j <= J; ++j) {
                /* c[j] = c[j-1] * (ek + j - 1) / j, exact without overflow */
                seq_t jj = (seq_t)j;
                seq_t g = gcd(c[j - 1], jj);
                if (g == (seq_t)0) g = (seq_t)1;
                c[j] = (c[j - 1] / g) * ((ek + (seq_t)(j - 1)) / (jj / g));
            }
            for (m = N; m >= k; --m) {
                seq_t sum = r[m];
                idx_t t = m / k;
                for (j = 1; j <= t; ++j) sum += c[j] * r[m - j * k];
                r[m] = sum;
            }
        }
        mem_free(c);
    }

    mem_free(h);
    mem_free(e);
    if (s_a) mem_free(s_a);
    s_a = r;
    s_max = N;
    s_init = 1;
}

seq_t a(idx_t n)
{
    if (!s_init || n > s_max) {
        idx_t N = n;
        if (s_init && 2 * s_max > N) N = 2 * s_max;
        if (N < 16) N = 16;
        A307604_build(N);
    }
    return s_a[n];
}
