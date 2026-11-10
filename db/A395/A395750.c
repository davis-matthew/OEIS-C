/*
 * A395750  a(n) are the terms occurring for the first time exactly n times
 *          consecutively in A000793.
 * https://oeis.org/A395750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Run the knapsack for Landau's function g(s) = A000793(s)
 *            on long-double logarithms, using only primes p <=
 *            1.5*sqrt(N*log(N)) + 30 (the largest prime factor of g(n) is at
 *            most 1.265*sqrt(n*log(n))). Find the first maximal run of equal
 *            values whose length is n. Then rebuild that g(m) exact...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 24 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

#define A395750_MAXLEN 256
#define A395750_NMAX ((idx_t)1 << 21)

static bool a395750_have[A395750_MAXLEN];
static idx_t a395750_start[A395750_MAXLEN];
static bool a395750_vok[A395750_MAXLEN];
static seq_t *a395750_val = NULL;
static idx_t a395750_N = 0;

static idx_t a395750_bound(idx_t N) {
    double x = (double)N;
    idx_t B;
    if (N < 3) return N;
    B = (idx_t)(1.5 * sqrt(x * log(x))) + 30;
    if (B > N) B = N;
    return B;
}

static idx_t *a395750_primes(idx_t B, size_t *cnt) {
    bool *comp = (bool *)calloc((size_t)(B + 2), sizeof(bool));
    idx_t *pr = (idx_t *)malloc(sizeof(idx_t) * (size_t)(B + 2));
    size_t c = 0;
    idx_t p, i;
    for (p = 2; p <= B; p++) {
        if (comp[p]) continue;
        pr[c++] = p;
        for (i = p * p; i <= B; i += p) comp[i] = true;
    }
    free(comp);
    *cnt = c;
    return pr;
}

static void a395750_scan(idx_t N) {
    idx_t B = a395750_bound(N);
    size_t np, t;
    idx_t *pr = a395750_primes(B, &np);
    long double *lg = (long double *)calloc((size_t)(N + 1), sizeof(long double));
    idx_t s, i, j;
    for (t = 0; t < np; t++) {
        idx_t p = pr[t];
        long double lp = logl((long double)p);
        for (s = N; s >= p; s--) {
            long double best = lg[s];
            idx_t pk = p;
            int k = 1;
            while (pk <= s) {
                long double c = lg[s - pk] + (long double)k * lp;
                if (c > best) best = c;
                if (pk > N / p) break;
                pk *= p;
                k++;
            }
            lg[s] = best;
        }
    }
    free(pr);
    i = 0;
    while (i < N) {
        idx_t len;
        j = i;
        while (j < N && lg[j + 1] - lg[j] < 1e-9L) j++;
        if (j >= N) break;
        len = j - i + 1;
        if (len < A395750_MAXLEN && !a395750_have[len]) {
            a395750_have[len] = true;
            a395750_start[len] = i;
        }
        i = j + 1;
    }
    free(lg);
    a395750_N = N;
}

static seq_t a395750_exact(idx_t m) {
    idx_t B, s;
    size_t np, t;
    idx_t *pr;
    long double *lg;
    unsigned char *ch;
    seq_t r = 1;
    if (m < 2) return (seq_t)1;
    B = a395750_bound(m);
    pr = a395750_primes(B, &np);
    lg = (long double *)calloc((size_t)(m + 1), sizeof(long double));
    ch = (unsigned char *)calloc(np * (size_t)(m + 1), 1);
    for (t = 0; t < np; t++) {
        idx_t p = pr[t];
        long double lp = logl((long double)p);
        unsigned char *row = ch + t * (size_t)(m + 1);
        for (s = m; s >= p; s--) {
            long double best = lg[s];
            unsigned char e = 0;
            idx_t pk = p;
            int k = 1;
            while (pk <= s) {
                long double c = lg[s - pk] + (long double)k * lp;
                if (c > best) { best = c; e = (unsigned char)k; }
                if (pk > m / p) break;
                pk *= p;
                k++;
            }
            lg[s] = best;
            row[s] = e;
        }
    }
    s = m;
    for (t = np; t-- > 0;) {
        unsigned char e = ch[t * (size_t)(m + 1) + s];
        if (e) {
            idx_t pk = 1;
            int k;
            for (k = 0; k < e; k++) pk *= pr[t];
            r *= (seq_t)pk;
            s -= pk;
        }
    }
    free(ch);
    free(lg);
    free(pr);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0 || n >= A395750_MAXLEN) return 0;
    if (a395750_val == NULL) a395750_val = mem_new(seq_t, A395750_MAXLEN);
    if (a395750_N == 0) a395750_scan(1024);
    while (!a395750_have[n] && a395750_N < A395750_NMAX)
        a395750_scan(a395750_N * 2);
    if (!a395750_have[n]) return 0;
    if (!a395750_vok[n]) {
        a395750_val[n] = a395750_exact(a395750_start[n]);
        a395750_vok[n] = true;
    }
    return a395750_val[n];
}
