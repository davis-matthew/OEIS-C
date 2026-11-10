/*
 * A296459  Decimal expansion of limiting power-ratio for A296284; see
 *          Comments.
 * https://oeis.org/A296459
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th digit of L = lim_{k->oo} A296284(k)/phi^k,
 *            phi=(1+sqrt(5))/2, with
 *            A296284(k)=A296284(k-1)+A296284(k-2)+k*b(k-2), A296284(0)=1,
 *            A296284(1)=2, b the increasing complement of A296284 (b(0)=3).
 *            Computed with exact big integers: L ~ A(N)/(F(N)*phi+F(N-1)),
 *            phi ~ F(N+1)/F(N), long division...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static size_t g_L;

static void bn_mulsmall(uint32_t *x, uint64_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < g_L; i++) {
        unsigned __int128 v = (unsigned __int128)x[i] * m + carry;
        x[i] = (uint32_t)v;
        carry = (uint64_t)(v >> 32);
    }
}
static void bn_add(uint32_t *r, const uint32_t *x, const uint32_t *y) {
    uint64_t c = 0;
    for (size_t i = 0; i < g_L; i++) {
        uint64_t s = (uint64_t)x[i] + y[i] + c;
        r[i] = (uint32_t)s;
        c = s >> 32;
    }
}
static void bn_sub(uint32_t *a, const uint32_t *b) {
    int64_t br = 0;
    for (size_t i = 0; i < g_L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - br;
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)v;
    }
}
static int bn_cmp(const uint32_t *a, const uint32_t *b) {
    for (size_t i = g_L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
static void bn_mul(uint32_t *r, const uint32_t *x, const uint32_t *y) {
    uint32_t *t = (uint32_t *)calloc(g_L, sizeof(uint32_t));
    for (size_t i = 0; i < g_L; i++) {
        if (!x[i]) continue;
        uint64_t carry = 0;
        for (size_t j = 0; i + j < g_L; j++) {
            uint64_t v = (uint64_t)x[i] * y[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)v;
            carry = v >> 32;
        }
    }
    memcpy(r, t, g_L * sizeof(uint32_t));
    free(t);
}

static uint8_t *A296459_digits = NULL;
static size_t A296459_have = 0;

static void A296459_compute(size_t D) {
    size_t N = 5 * D + 60;
    g_L = N / 16 + 8;
    size_t L = g_L;
    uint64_t CAP = 1000000000000ULL;

    uint64_t *sm = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *bb = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    size_t blen = 0, aidx = 0;
    uint64_t cand = 1;
    sm[0] = 1; sm[1] = 2;
    /* generate b(0) */
    while (aidx < 2 && sm[aidx] <= cand) {
        if (sm[aidx] == cand) cand++;
        aidx++;
    }
    bb[blen++] = cand; cand++;

    uint32_t *p2 = (uint32_t *)calloc(L, 4);
    uint32_t *p1 = (uint32_t *)calloc(L, 4);
    uint32_t *cu = (uint32_t *)calloc(L, 4);
    uint32_t *tmp = (uint32_t *)calloc(L, 4);
    p2[0] = 1; p1[0] = 2;
    for (size_t i = 2; i <= N; i++) {
        while (blen <= i - 2) {
            uint64_t c = cand;
            while (aidx < i && sm[aidx] <= c) {
                if (sm[aidx] == c) c++;
                aidx++;
            }
            bb[blen++] = c; cand = c + 1;
        }
        uint64_t add = (uint64_t)i * bb[i - 2];
        uint64_t s = sm[i - 1] + sm[i - 2] + add;
        sm[i] = s > CAP ? CAP : s;
        bn_add(cu, p1, p2);
        memset(tmp, 0, L * 4);
        tmp[0] = (uint32_t)add; tmp[1] = (uint32_t)(add >> 32);
        bn_add(cu, cu, tmp);
        uint32_t *t = p2; p2 = p1; p1 = cu; cu = t;
    }
    /* p1 = A(N) */
    uint32_t *fa = (uint32_t *)calloc(L, 4);   /* F(k-1) */
    uint32_t *fb = (uint32_t *)calloc(L, 4);   /* F(k) */
    uint32_t *fc = (uint32_t *)calloc(L, 4);
    fa[0] = 0; fb[0] = 1;                       /* F0, F1 */
    for (size_t k = 1; k < N; k++) {            /* ends with fa=F(N-1), fb=F(N) */
        bn_add(fc, fa, fb);
        uint32_t *t = fa; fa = fb; fb = fc; fc = t;
    }
    uint32_t *fn1 = (uint32_t *)calloc(L, 4);   /* F(N+1) */
    bn_add(fn1, fa, fb);
    uint32_t *num = (uint32_t *)calloc(L, 4);
    uint32_t *den = (uint32_t *)calloc(L, 4);
    uint32_t *t2 = (uint32_t *)calloc(L, 4);
    bn_mul(num, p1, fb);
    bn_mul(den, fb, fn1);
    bn_mul(t2, fa, fb);
    bn_add(den, den, t2);

    uint8_t *out = (uint8_t *)malloc(D + 2);
    uint32_t *r = num;
    unsigned q = 0;
    while (bn_cmp(r, den) >= 0) { bn_sub(r, den); q++; }
    out[0] = (uint8_t)(q / 10);
    out[1] = (uint8_t)(q % 10);
    for (size_t i = 2; i < D + 2; i++) {
        bn_mulsmall(r, 10);
        unsigned d = 0;
        while (bn_cmp(r, den) >= 0) { bn_sub(r, den); d++; }
        out[i] = (uint8_t)d;
    }

    free(sm); free(bb); free(p2); free(p1); free(cu); free(tmp);
    free(fa); free(fb); free(fc); free(fn1); free(num); free(den); free(t2);
    free(A296459_digits);
    A296459_digits = out;
    A296459_have = D + 2;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    size_t need = (size_t)n - 1;  /* digits stored at index n-2 */
    if (need > A296459_have) {
        size_t D = A296459_have * 2;
        if (D < need + 20) D = need + 20;
        if (D < 64) D = 64;
        A296459_compute(D);
    }
    return (seq_t)A296459_digits[(size_t)n - 2];
}
