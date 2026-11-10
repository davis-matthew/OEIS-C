/*
 * A271881  Decimal expansion of the difference between 1/5 and the
 *          probability that a random real number is evil.
 * https://oeis.org/A271881
 *
 * Offset:    -63    (a(-63) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   a(n) = digit of x = 1/5 - A100061(666)/9^666 at 10^-(n+127),
 *            i.e. floor(x*10^(n+127)) mod 10, where A100061(k) = sum_{j=1..9}
 *            9^(j-1)*A100061(k-j), A100061(1..9) = 1,10,100,...,10^8.
 *            Computed exactly with a native base-1e9 bignum and long division
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 121 terms through n = 1990 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BL 100
#define BB 1000000000u

typedef struct { uint32_t d[BL]; } BigX;

static void bx_mul(BigX *a, uint32_t m) {
    uint64_t c = 0;
    for (int i = 0; i < BL; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)(t % BB);
        c = t / BB;
    }
}
static void bx_add(BigX *a, const BigX *b) {
    uint32_t c = 0;
    for (int i = 0; i < BL; i++) {
        uint32_t t = a->d[i] + b->d[i] + c;
        if (t >= BB) { t -= BB; c = 1; } else c = 0;
        a->d[i] = t;
    }
}
static void bx_sub(BigX *a, const BigX *b) { /* a >= b */
    int br = 0;
    for (int i = 0; i < BL; i++) {
        int64_t t = (int64_t)a->d[i] - b->d[i] - br;
        if (t < 0) { t += BB; br = 1; } else br = 0;
        a->d[i] = (uint32_t)t;
    }
}
static int bx_cmp(const BigX *a, const BigX *b) {
    for (int i = BL - 1; i >= 0; i--) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

static int A271881_ready = 0;
static BigX A271881_r, A271881_M;
static unsigned char *A271881_dig = NULL;
static long long A271881_cnt = 0, A271881_cap = 0;

static void A271881_init(void) {
    const int K = 666;
    BigX *A = (BigX *)malloc(sizeof(BigX) * (K + 1));
    if (!A) exit(1);
    memset(A, 0, sizeof(BigX) * (K + 1));
    uint32_t pw[9];
    pw[0] = 1;
    for (int k = 1; k < 9; k++) pw[k] = pw[k - 1] * 9u;
    uint32_t v = 1;
    for (int i = 1; i <= 9; i++) { A[i].d[0] = v % BB; A[i].d[1] = v / BB; v *= 10u; }
    for (int i = 10; i <= K; i++) {
        for (int k = 1; k <= 9; k++) {
            BigX t = A[i - k];
            bx_mul(&t, pw[k - 1]);
            bx_add(&A[i], &t);
        }
    }
    BigX P; memset(&P, 0, sizeof P); P.d[0] = 1;
    for (int i = 0; i < K; i++) bx_mul(&P, 9u);
    BigX N5 = A[K];
    bx_mul(&N5, 5u);
    A271881_r = P;
    bx_sub(&A271881_r, &N5);
    A271881_M = P;
    bx_mul(&A271881_M, 5u);
    free(A);
    A271881_ready = 1;
}

seq_t a(idx_t n) {
    if (!A271881_ready) A271881_init();
    long long p = (long long)n + 127;
    if (p < 1) return (seq_t)0;
    if (p > A271881_cap) {
        long long nc = p + 256;
        unsigned char *t = (unsigned char *)realloc(A271881_dig, (size_t)nc);
        if (!t) exit(1);
        A271881_dig = t;
        A271881_cap = nc;
    }
    while (A271881_cnt < p) {
        bx_mul(&A271881_r, 10u);
        int q = 0;
        while (bx_cmp(&A271881_r, &A271881_M) >= 0) {
            bx_sub(&A271881_r, &A271881_M);
            q++;
        }
        A271881_dig[A271881_cnt++] = (unsigned char)q;
    }
    return (seq_t)A271881_dig[p - 1];
}
