/*
 * A366293  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A365711(i) = A365711(j) for all i, j >= 1, where A365711 is the
 *          Dirichlet inverse of balanced ternary enumeration of integers
 *          (A117966).
 * https://oeis.org/A366293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of c = A365711,
 *            where c(1)=1, c(n) = -Sum_{d|n, d>1} A117966(d)*c(n/d)
 *            (Dirichlet inverse of A117966); computed by sieve, values
 *            compared mod 2^128
 * Range:     u8 n <= 364, u16 n <= 59049, u32 n <= 59049, u64 n <= 59049,
 *            u128 n <= 59049, big any n
 * Validated: standard, pass, 106 terms through n = 8874 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 a366293_u128;

static uint32_t *a366293_res = 0;
static uint64_t a366293_cap = 0;

static long long a366293_bal(uint64_t j)
{
    long long val = 0, p = 1;
    while (j) {
        int d = (int)(j % 3);
        if (d == 1) val += p;
        else if (d == 2) val -= p;
        p *= 3;
        j /= 3;
    }
    return val;
}

static void a366293_build(uint64_t N)
{
    a366293_u128 *c = (a366293_u128 *)calloc(N + 1, sizeof(a366293_u128));
    long long *b = (long long *)malloc((N + 1) * sizeof(long long));
    uint64_t *reps = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    uint32_t *res = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint64_t j, m, k, i;
    uint64_t nrep = 0;
    for (j = 1; j <= N; j++) b[j] = a366293_bal(j);
    c[1] = 1;
    for (m = 1; m <= N; m++) {
        a366293_u128 cm = c[m];
        if (cm == 0) continue;
        for (j = 2, k = 2 * m; k <= N; j++, k += m) {
            c[k] -= (a366293_u128)(__int128)b[j] * cm;
        }
    }
    res[0] = 0;
    for (i = 1; i <= N; i++) {
        uint64_t t;
        int found = 0;
        for (t = 0; t < nrep; t++) {
            if (c[reps[t]] == c[i]) { res[i] = (uint32_t)(t + 1); found = 1; break; }
        }
        if (!found) {
            reps[nrep++] = i;
            res[i] = (uint32_t)nrep;
        }
    }
    free(c);
    free(b);
    free(reps);
    if (a366293_res) free(a366293_res);
    a366293_res = res;
    a366293_cap = N;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn > a366293_cap) {
        uint64_t N = nn;
        if (N < 2 * a366293_cap) N = 2 * a366293_cap;
        if (N < 1000) N = 1000;
        a366293_build(N);
    }
    return (seq_t)a366293_res[nn];
}
