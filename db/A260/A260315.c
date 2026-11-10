/*
 * A260315  Decimal expansion of the generating constant c of A139244.
 * https://oeis.org/A260315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   c = lim_{k->inf} A139244(k)^(1/2^(k+1)), where A139244(0) = 4,
 *            A139244(k) = A139244(k-1)^2 - 1; a(n) = n-th decimal digit of c
 *            (a(1) = 1). Computed with private fixed-point big integers
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t A260315_u32;

static void A260315_mul(const A260315_u32 *x, int lx, const A260315_u32 *y, int ly,
                        A260315_u32 *out, int M)
{
    for (int i = 0; i < M; i++) out[i] = 0;
    for (int i = 0; i < lx; i++) {
        uint64_t carry = 0;
        if (x[i] == 0) continue;
        for (int j = 0; j < ly && i + j < M; j++) {
            uint64_t t = (uint64_t)x[i] * y[j] + out[i + j] + carry;
            out[i + j] = (A260315_u32)t;
            carry = t >> 32;
        }
        int k = i + ly;
        while (carry && k < M) {
            uint64_t t = (uint64_t)out[k] + carry;
            out[k] = (A260315_u32)t;
            carry = t >> 32;
            k++;
        }
    }
}

static int A260315_cmp(const A260315_u32 *x, const A260315_u32 *y, int M)
{
    for (int i = M - 1; i >= 0; i--) {
        if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    long long D = (long long)n + 30;
    int FL = (int)((D * 10 / 3 + 64) / 32 + 1);
    long long F = 32LL * FL;
    long long need = (D * 10 / 3 + 64) * 100;
    long long M2 = 2;
    int N = 0;
    while (M2 * 97 < need) { M2 *= 2; N++; }
    int K = (int)((M2 + 2 * F + 2) / 64 + 2);
    int M = 2 * K;

    A260315_u32 *A = (A260315_u32 *)calloc((size_t)M, sizeof(A260315_u32));
    A260315_u32 *tmp = (A260315_u32 *)calloc((size_t)M, sizeof(A260315_u32));
    A260315_u32 *T = (A260315_u32 *)calloc((size_t)M, sizeof(A260315_u32));
    A260315_u32 *r = (A260315_u32 *)calloc((size_t)M, sizeof(A260315_u32));
    A260315_u32 *sq = (A260315_u32 *)calloc((size_t)M, sizeof(A260315_u32));

    A[0] = 4;
    for (int s = 0; s < N; s++) {
        A260315_mul(A, M, A, M, tmp, M);
        int i = 0;
        while (i < M && tmp[i] == 0) { tmp[i] = 0xFFFFFFFFu; i++; }
        if (i < M) tmp[i]--;
        for (int k = 0; k < M; k++) A[k] = tmp[k];
    }

    /* X = A * 2^F (fixed point, F fractional bits) */
    A260315_u32 *X = (A260315_u32 *)calloc((size_t)M, sizeof(A260315_u32));
    for (int i = 0; i + FL < M; i++) X[i + FL] = A[i];

    for (int step = 0; step < N + 1; step++) {
        for (int i = 0; i < M; i++) T[i] = 0;
        for (int i = 0; i + FL < M; i++) T[i + FL] = X[i];
        for (int i = 0; i < M; i++) r[i] = 0;
        for (int bit = 32 * K - 1; bit >= 0; bit--) {
            r[bit / 32] |= (A260315_u32)1 << (bit % 32);
            A260315_mul(r, K, r, K, sq, M);
            if (A260315_cmp(sq, T, M) > 0)
                r[bit / 32] &= ~((A260315_u32)1 << (bit % 32));
        }
        for (int i = 0; i < M; i++) X[i] = (i < K) ? r[i] : 0;
    }

    /* fractional part: low FL limbs of X; extract digits */
    A260315_u32 *fr = (A260315_u32 *)calloc((size_t)FL, sizeof(A260315_u32));
    for (int i = 0; i < FL; i++) fr[i] = X[i];
    unsigned digit = 0;
    for (idx_t d = 1; d < n; d++) {
        uint64_t carry = 0;
        for (int i = 0; i < FL; i++) {
            uint64_t t = (uint64_t)fr[i] * 10 + carry;
            fr[i] = (A260315_u32)t;
            carry = t >> 32;
        }
        digit = (unsigned)carry;
    }

    free(A); free(tmp); free(T); free(r); free(sq); free(X); free(fr);
    return (seq_t)digit;
}
