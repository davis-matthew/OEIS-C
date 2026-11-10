/*
 * A348584  Numbers k such that k | A328258(k).
 * https://oeis.org/A348584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k with k | A328258(k), A328258(k) = Prod_{p^e||k} (1
 *            - (-1)^p*p^e). Computed by search: k = 2^a*m'*Q with Q the
 *            largest odd prime power; Q is solved exactly from
 *            (2^a-1)*U(m')*(1+Q) = j*2^a*m'*Q, so Q = N/(j*2^a*m' - N), with
 *            N = (2^a-1)*U(m')
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A348584_MAXT 2048

static uint64_t A348584_terms[A348584_MAXT];
static size_t   A348584_cnt = 0;
static uint64_t A348584_done_L = 0;

static uint64_t A348584_L, A348584_X, A348584_pa, A348584_am1;
static uint64_t *A348584_pr = 0;
static size_t A348584_np = 0;
static uint64_t A348584_found[A348584_MAXT];
static size_t A348584_nf = 0;

/* Q odd >= 3: is it a power of a prime greater than P? */
static bool A348584_ppower_gt(uint64_t Q, uint64_t P)
{
    uint64_t s = 0;
    for (uint64_t d = 3; d * d <= Q; d += 2) {
        if (Q % d == 0) { s = d; break; }
    }
    if (s == 0) s = Q;
    while (Q % s == 0) Q /= s;
    return Q == 1 && s > P;
}

static void A348584_rec(uint64_t m, uint64_t U, uint64_t P, size_t start)
{
    uint64_t N = A348584_am1 * U;
    uint64_t den = A348584_pa * m;
    uint64_t j = N / den + 1;
    for (;; ++j) {
        uint64_t D = j * den - N;
        if (D * 3 > N) break;
        if (N % D == 0) {
            uint64_t Q = N / D;
            if ((Q & 1) && Q >= 3 && Q <= A348584_X / m && A348584_ppower_gt(Q, P)) {
                if (A348584_nf < A348584_MAXT)
                    A348584_found[A348584_nf++] = den * Q;
            }
        }
    }
    for (size_t i = start; i < A348584_np; ++i) {
        uint64_t r = A348584_pr[i];
        if (m * r * (r + 1) > A348584_X) break;
        uint64_t pk = r;
        while (m * pk * (r + 1) <= A348584_X) {
            A348584_rec(m * pk, U * (1 + pk), r, i + 1);
            pk *= r;
        }
    }
}

static int A348584_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

static void A348584_run(uint64_t L)
{
    uint64_t lim = 2;
    while (lim * lim <= L) ++lim;
    lim += 2;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    A348584_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 2 + 2));
    A348584_np = 0;
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (!comp[i]) {
            A348584_pr[A348584_np++] = i;
            for (uint64_t k = i * i; k <= lim; k += 2 * i) comp[k] = 1;
        }
    }
    free(comp);

    A348584_nf = 0;
    A348584_L = L;
    for (unsigned a = 1; (3ULL << a) <= L; ++a) {
        A348584_pa = 1ULL << a;
        A348584_am1 = A348584_pa - 1;
        A348584_X = L >> a;
        A348584_rec(1, 1, 1, 0);
    }
    free(A348584_pr);
    A348584_pr = 0;

    qsort(A348584_found, A348584_nf, sizeof(uint64_t), A348584_cmp);
    A348584_cnt = 0;
    A348584_terms[A348584_cnt++] = 1;
    for (size_t i = 0; i < A348584_nf; ++i) {
        if (A348584_cnt < A348584_MAXT &&
            A348584_terms[A348584_cnt - 1] != A348584_found[i])
            A348584_terms[A348584_cnt++] = A348584_found[i];
    }
    A348584_done_L = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A348584_done_L == 0 || A348584_cnt < (size_t)n) {
        uint64_t L = 300000000000ULL;
        while (L > 20000) L /= 8;
        while (L <= A348584_done_L) L *= 8;
        for (;;) {
            A348584_run(L);
            if (A348584_cnt >= (size_t)n) break;
            if (L > (1ULL << 57) || A348584_cnt >= A348584_MAXT) break;
            L *= 8;
        }
    }
    if ((size_t)n > A348584_cnt) return (seq_t)0;
    return (seq_t)A348584_terms[n - 1];
}
