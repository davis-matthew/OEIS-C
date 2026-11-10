/*
 * A366382  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A349134(i) = A349134(j) for all i, j >= 1, where A349134 is
 *          Dirichlet inverse of Kimberling's paraphrase of the binary number
 *          system (A003602).
 * https://oeis.org/A366382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of first-appearance class of A349134(n), where
 *            A349134 is the Dirichlet inverse of A003602 (b(1)=1, b(n) =
 *            -Sum_{d|n, d>1} A003602(d)*b(n/d)); computed by a sieve, with
 *            values compared via two independent residues (mod 2^64 and mod
 *            2^61-1)
 * Range:     u8 n <= 654, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9844 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A366382_P ((((uint64_t)1) << 61) - 1)

static uint64_t *A366382_h1 = 0;
static uint64_t *A366382_h2 = 0;
static uint64_t *A366382_lab = 0;
static uint64_t A366382_N = 0;

static uint64_t A366382_mulp(uint64_t x, uint64_t y) {
    unsigned __int128 t = (unsigned __int128)x * y;
    return (uint64_t)(t % A366382_P);
}

static int A366382_cmp(const void *x, const void *y) {
    uint64_t i = *(const uint64_t *)x, j = *(const uint64_t *)y;
    if (A366382_h1[i] != A366382_h1[j]) return A366382_h1[i] < A366382_h1[j] ? -1 : 1;
    if (A366382_h2[i] != A366382_h2[j]) return A366382_h2[i] < A366382_h2[j] ? -1 : 1;
    if (i != j) return i < j ? -1 : 1;
    return 0;
}

static void A366382_build(uint64_t N) {
    uint64_t i, m, d;
    uint64_t *acc1 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *acc2 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *Av = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *idx = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    unsigned char *first = (unsigned char *)calloc(N + 1, 1);
    free(A366382_h1); free(A366382_h2); free(A366382_lab);
    A366382_h1 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    A366382_h2 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    A366382_lab = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    for (i = 1; i <= N; i++) {
        uint64_t o = i;
        while ((o & 1) == 0) o >>= 1;
        Av[i] = (o + 1) >> 1;
    }
    for (m = 1; m <= N; m++) {
        if (m == 1) {
            A366382_h1[1] = 1;
            A366382_h2[1] = 1;
        } else {
            A366382_h1[m] = (uint64_t)0 - acc1[m];
            A366382_h2[m] = (A366382_P - acc2[m]) % A366382_P;
        }
        for (d = 2; m * d <= N; d++) {
            acc1[m * d] += Av[d] * A366382_h1[m];
            acc2[m * d] = (acc2[m * d] + A366382_mulp(Av[d], A366382_h2[m])) % A366382_P;
        }
    }
    for (i = 1; i <= N; i++) idx[i - 1] = i;
    qsort(idx, N, sizeof(uint64_t), A366382_cmp);
    for (i = 0; i < N;) {
        uint64_t j = i;
        first[idx[i]] = 1;
        while (j < N && A366382_h1[idx[j]] == A366382_h1[idx[i]] &&
               A366382_h2[idx[j]] == A366382_h2[idx[i]]) j++;
        /* idx[i] is the smallest index of the group (sorted by index) */
        {
            uint64_t k;
            for (k = i; k < j; k++) A366382_lab[idx[k]] = idx[i];
        }
        i = j;
    }
    {
        uint64_t cnt = 0;
        uint64_t *rank = idx; /* reuse: rank[first index] = label */
        for (i = 1; i <= N; i++) {
            if (first[i]) { cnt++; rank[i] = cnt; }
        }
        for (i = 1; i <= N; i++) A366382_lab[i] = rank[A366382_lab[i]];
    }
    A366382_N = N;
    free(acc1); free(acc2); free(Av); free(idx); free(first);
}

seq_t a(idx_t n) {
    if ((uint64_t)n > A366382_N) {
        uint64_t N = 10000;
        if ((uint64_t)n > N) N = (uint64_t)n;
        if (N < 2 * A366382_N && 2 * A366382_N >= (uint64_t)n) N = (uint64_t)n;
        A366382_build(N);
    }
    return (seq_t)A366382_lab[n];
}
