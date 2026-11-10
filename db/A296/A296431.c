/*
 * A296431  Decimal expansion of ratio-sum for A296278; see Comments.
 * https://oeis.org/A296431
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th digit of S = Sum_{k>=1} |A296278(k)/A296278(k-1) -
 *            phi|, phi=(1+sqrt(5))/2; computed by exact big-integer long
 *            division of the ratios, truncated to fixed-point decimal digits
 *            and summed
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

#define RS_BASE 1000000000u

/* ---- digits of golden ratio (A001622), renamed ---- */
static int phi_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
#define PHI_BASE 1000000000u
static void phi_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % PHI_BASE);
        carry = v / PHI_BASE;
    }
}
static void phi_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % PHI_BASE);
        carry = s / PHI_BASE;
    }
}
static void phi_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += PHI_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}
static uint8_t *phi_digits = NULL;
static size_t phi_have = 0;
static void phi_compute(size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *s = (uint8_t *)malloc(M);
    uint8_t *out = (uint8_t *)malloc(M);
    for (size_t i = 0; i < M; i++) {
        phi_mul_small(r, L, 100);
        phi_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        phi_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            phi_add_small(t, L, mid);
            phi_mul_small(t, L, mid);
            if (phi_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        phi_add_small(t, L, lo);
        phi_mul_small(t, L, lo);
        phi_sub(r, t, L);
        phi_mul_small(p, L, 10);
        phi_add_small(p, L, lo);
        s[i] = (uint8_t)lo;
    }
    s[0] += 1;
    unsigned rem = 0;
    for (size_t i = 0; i < M; i++) {
        unsigned v = rem * 10 + s[i];
        out[i] = (uint8_t)(v / 2);
        rem = v % 2;
    }
    free(p); free(r); free(d); free(t); free(s);
    free(phi_digits);
    phi_digits = out;
    phi_have = M;
}
static int A001622_digit(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n + 5 > phi_have) {
        size_t M = phi_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 64) M = 64;
        phi_compute(M);
    }
    return (int)phi_digits[n - 1];
}

/* ---- big integer helpers (base 1e9, native limbs) ---- */
static int rs_cmp(const uint32_t *a, const uint32_t *b, size_t n) {
    for (size_t i = n; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
static void rs_sub(uint32_t *a, const uint32_t *b, size_t n) {
    int64_t borrow = 0;
    for (size_t i = 0; i < n; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += RS_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}
static void rs_add(uint32_t *a, const uint32_t *b, size_t n) {
    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + carry;
        a[i] = (uint32_t)(s % RS_BASE);
        carry = s / RS_BASE;
    }
}
static void rs_add_small(uint32_t *a, size_t n, uint64_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < n && carry; i++) {
        uint64_t s = (uint64_t)a[i] + carry;
        a[i] = (uint32_t)(s % RS_BASE);
        carry = s / RS_BASE;
    }
}
static void rs_mul10(uint32_t *a, size_t n) {
    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t v = (uint64_t)a[i] * 10 + carry;
        a[i] = (uint32_t)(v % RS_BASE);
        carry = v / RS_BASE;
    }
}
static int rs_in_sorted(const uint64_t *arr, size_t cnt, uint64_t v) {
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] == v) return 1;
        if (arr[mid] < v) lo = mid + 1; else hi = mid;
    }
    return 0;
}

static unsigned char *rs_digits = NULL;
static size_t rs_have = 0;

static void rs_compute(size_t M) {
    size_t P = M + 30;
    size_t W = P + 3;
    size_t N = 5 * P + 200;
    size_t Lmax = N / 30 + 12;
    unsigned char *PH = (unsigned char *)calloc(W, 1);
    unsigned char *R = (unsigned char *)calloc(W, 1);
    unsigned char *S = (unsigned char *)calloc(W, 1);
    unsigned char *D = (unsigned char *)calloc(W, 1);
    uint64_t *bs = (uint64_t *)calloc(N + 3, sizeof(uint64_t));
    uint64_t *as = (uint64_t *)calloc(N + 3, sizeof(uint64_t));
    uint32_t *X0 = (uint32_t *)calloc(Lmax, sizeof(uint32_t));
    uint32_t *X1 = (uint32_t *)calloc(Lmax, sizeof(uint32_t));
    uint32_t *X2 = (uint32_t *)calloc(Lmax, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(Lmax, sizeof(uint32_t));

    PH[2] = 1;
    for (size_t j = 1; j <= P; j++) PH[2 + j] = (unsigned char)A001622_digit((idx_t)(j + 1));

    X0[0] = 1; X1[0] = 2;
    bs[0] = 3; bs[1] = 4; as[0] = 1; as[1] = 2;
    size_t len1 = 1;

    for (size_t k = 1; k <= N; k++) {
        size_t n = len1 + 1;
        memcpy(r, X1, n * sizeof(uint32_t));
        unsigned q = 0;
        while (rs_cmp(r, X0, n) >= 0) { rs_sub(r, X0, n); q++; }
        memset(R, 0, W);
        R[0] = (unsigned char)((q / 100) % 10);
        R[1] = (unsigned char)((q / 10) % 10);
        R[2] = (unsigned char)(q % 10);
        for (size_t j = 0; j < P; j++) {
            rs_mul10(r, n);
            unsigned char d = 0;
            while (rs_cmp(r, X0, n) >= 0) { rs_sub(r, X0, n); d++; }
            R[3 + j] = d;
        }
        const unsigned char *hi = R, *lo = PH;
        if (memcmp(R, PH, W) < 0) { hi = PH; lo = R; }
        int borrow = 0;
        for (size_t i = W; i > 0; i--) {
            int v = (int)hi[i - 1] - (int)lo[i - 1] - borrow;
            if (v < 0) { v += 10; borrow = 1; } else borrow = 0;
            D[i - 1] = (unsigned char)v;
        }
        int carry = 0;
        for (size_t i = W; i > 0; i--) {
            int v = (int)S[i - 1] + (int)D[i - 1] + carry;
            if (v >= 10) { v -= 10; carry = 1; } else carry = 0;
            S[i - 1] = (unsigned char)v;
        }

        if (k == N) break;
        size_t i = k + 1;
        uint64_t v = bs[i - 1] + 1;
        while (rs_in_sorted(as, i, v)) v++;
        bs[i] = v;
        uint64_t prod = bs[i - 2] * bs[i - 1] * bs[i];
        size_t n2 = len1 + 1;
        memcpy(X2, X1, n2 * sizeof(uint32_t));
        rs_add(X2, X0, n2);
        rs_add_small(X2, n2, prod);
        size_t len2 = n2;
        while (len2 > 1 && X2[len2 - 1] == 0) len2--;
        if (len2 > 2) as[i] = UINT64_MAX;
        else as[i] = (uint64_t)X2[0] + (len2 > 1 ? (uint64_t)X2[1] * RS_BASE : 0);
        uint32_t *tmp = X0; X0 = X1; X1 = X2; X2 = tmp;
        len1 = len2;
    }

    free(PH); free(R); free(D); free(bs); free(as);
    free(X0); free(X1); free(X2); free(r);
    free(rs_digits);
    rs_digits = S;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    if ((size_t)n + 2 > rs_have) {
        size_t M = rs_have * 2;
        if (M < (size_t)n + 60) M = (size_t)n + 60;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
