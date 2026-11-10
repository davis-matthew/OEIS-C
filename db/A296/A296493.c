/*
 * A296493  Decimal expansion of ratio-sum for A296555; see Comments.
 * https://oeis.org/A296493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th digit of Sum_{k>=1} |c(k)/c(k-1) - phi|,
 *            phi=(1+sqrt(5))/2, where c = A296555: c(n) = c(n-1) + c(n-2) +
 *            b(n) + n, c(0)=1, c(1)=2, b(0..2)=3,4,5, b = complement of c;
 *            computed with big fixed-point integers
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void r_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void r_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - br;
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)v;
    }
}

static int r_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

static void r_addu64(uint32_t *a, size_t L, uint64_t v) {
    uint64_t c = 0;
    for (size_t i = 0; i < L && (v || c); i++) {
        uint64_t s = (uint64_t)a[i] + (v & 0xffffffffu) + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
        v >>= 32;
    }
}

static void r_mul(uint32_t *r, const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = 0; i < L; i++) r[i] = 0;
    for (size_t i = 0; i < L; i++) {
        if (!a[i]) continue;
        uint64_t carry = 0;
        for (size_t j = 0; i + j < L; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + r[i + j] + carry;
            r[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
    }
}

static void r_shl(uint32_t *a, size_t L, size_t k) {
    size_t ls = k / 32, bs = k % 32;
    for (size_t i = L; i > 0; i--) {
        size_t idx = i - 1;
        uint32_t v = 0;
        if (idx >= ls) {
            v = a[idx - ls] << bs;
            if (bs && idx > ls) v |= a[idx - ls - 1] >> (32 - bs);
        }
        a[idx] = v;
    }
}

static void r_mulsmall(uint32_t *a, size_t L, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static unsigned r_getbit(const uint32_t *a, size_t pos) {
    return (a[pos / 32] >> (pos % 32)) & 1u;
}

static void r_clearbit(uint32_t *a, size_t pos) {
    a[pos / 32] &= ~((uint32_t)1 << (pos % 32));
}

/* q = N / d (d nonzero); rem is scratch of at least L+1 limbs */
static void r_div(const uint32_t *N, const uint32_t *d, uint32_t *q, size_t L, uint32_t *rem) {
    size_t dl = L;
    while (dl > 1 && d[dl - 1] == 0) dl--;
    for (size_t i = 0; i <= dl; i++) rem[i] = 0;
    for (size_t i = 0; i < L; i++) q[i] = 0;
    for (size_t bit = 32 * L; bit > 0; bit--) {
        size_t pos = bit - 1;
        uint32_t carry = (N[pos / 32] >> (pos % 32)) & 1u;
        for (size_t k = 0; k <= dl; k++) {
            uint32_t nc = rem[k] >> 31;
            rem[k] = (rem[k] << 1) | carry;
            carry = nc;
        }
        int ge = 1;
        if (rem[dl] == 0) {
            for (size_t k = dl; k > 0; k--) {
                if (rem[k - 1] > d[k - 1]) { ge = 1; break; }
                if (rem[k - 1] < d[k - 1]) { ge = 0; break; }
            }
        }
        if (ge) {
            int64_t br = 0;
            for (size_t k = 0; k <= dl; k++) {
                int64_t v = (int64_t)rem[k] - (k < dl ? (int64_t)d[k] : 0) - br;
                if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
                rem[k] = (uint32_t)v;
            }
            q[pos / 32] |= (uint32_t)1 << (pos % 32);
        }
    }
}

static uint8_t *A296493_digits = NULL;
static size_t A296493_have = 0;

static void A296493_compute(size_t M) {
    size_t P = M * 10 / 3 + 70;
    size_t NT = P * 29 / 20 + 100;
    size_t L = (P * 5 / 2 + 300) / 32 + 4;
    const uint64_t SAT = (uint64_t)1 << 62;

    /* complement sequence b with saturated small copies of c */
    uint64_t *b = (uint64_t *)calloc(NT + 5, sizeof(uint64_t));
    uint64_t *as = (uint64_t *)calloc(NT + 5, sizeof(uint64_t));
    b[0] = 3; b[1] = 4; b[2] = 5;
    as[0] = 1; as[1] = 2;
    {
        size_t pa = 0, pb = 0;
        uint64_t nc = 6;
        for (size_t i = 2; i <= NT + 1; i++) {
            uint64_t s = as[i - 1] + as[i - 2] + b[i] + i;
            if (s > SAT) s = SAT;
            as[i] = s;
            uint64_t cand = nc;
            for (;;) {
                while (pa <= i && as[pa] < cand) ++pa;
                while (pb <= i && b[pb] < cand) ++pb;
                int used = (pa <= i && as[pa] == cand) || (pb <= i && b[pb] == cand);
                if (!used) break;
                ++cand;
            }
            b[i + 1] = cand;
            nc = cand + 1;
        }
    }

    uint32_t *T = (uint32_t *)calloc(L, 4);
    uint32_t *rt = (uint32_t *)calloc(L, 4);
    uint32_t *sq = (uint32_t *)calloc(L, 4);
    uint32_t *phi = (uint32_t *)calloc(L, 4);
    uint32_t *prev = (uint32_t *)calloc(L, 4);
    uint32_t *cur = (uint32_t *)calloc(L, 4);
    uint32_t *nxt = (uint32_t *)calloc(L, 4);
    uint32_t *X = (uint32_t *)calloc(L, 4);
    uint32_t *Y = (uint32_t *)calloc(L, 4);
    uint32_t *q = (uint32_t *)calloc(L, 4);
    uint32_t *rem = (uint32_t *)calloc(L + 2, 4);
    uint32_t *S = (uint32_t *)calloc(L, 4);
    uint8_t *out = (uint8_t *)malloc(M);

    /* sqrt(5) * 2^P = isqrt(5 * 2^(2P)) */
    T[0] = 5;
    r_shl(T, L, 2 * P);
    for (size_t bit = P + 3; bit + 1 > 0; bit--) {
        uint32_t save = rt[bit / 32];
        rt[bit / 32] |= (uint32_t)1 << (bit % 32);
        r_mul(sq, rt, rt, L);
        if (r_cmp(sq, T, L) > 0) rt[bit / 32] = save;
    }
    /* phi = (2^P + sqrt5)/2 */
    for (size_t i = 0; i < L; i++) phi[i] = rt[i];
    {
        uint32_t *one = sq;
        for (size_t i = 0; i < L; i++) one[i] = 0;
        one[P / 32] = (uint32_t)1 << (P % 32);
        r_add(phi, one, L);
        uint32_t carry = 0;
        for (size_t i = L; i > 0; i--) {
            uint32_t nc = phi[i - 1] & 1u;
            phi[i - 1] = (phi[i - 1] >> 1) | (carry << 31);
            carry = nc;
        }
    }

    prev[0] = 1;
    cur[0] = 2;
    for (size_t i = 1; i <= NT; i++) {
        for (size_t k = 0; k < L; k++) X[k] = cur[k];
        r_shl(X, L, P);
        r_mul(Y, phi, prev, L);
        if (r_cmp(X, Y, L) >= 0) r_sub(X, Y, L); else { r_sub(Y, X, L); for (size_t k = 0; k < L; k++) X[k] = Y[k]; }
        r_div(X, prev, q, L, rem);
        r_add(S, q, L);
        /* next term */
        for (size_t k = 0; k < L; k++) nxt[k] = cur[k];
        r_add(nxt, prev, L);
        r_addu64(nxt, L, b[i + 1] + (uint64_t)(i + 1));
        uint32_t *tmp = prev; prev = cur; cur = nxt; nxt = tmp;
    }

    {
        unsigned v = 0;
        for (unsigned j = 0; j < 8; j++) {
            v |= r_getbit(S, P + j) << j;
            r_clearbit(S, P + j);
        }
        out[0] = (uint8_t)v;
        for (size_t k = 1; k < M; k++) {
            r_mulsmall(S, L, 10);
            unsigned dg = 0;
            for (unsigned j = 0; j < 4; j++) {
                dg |= r_getbit(S, P + j) << j;
                r_clearbit(S, P + j);
            }
            out[k] = (uint8_t)dg;
        }
    }

    free(b); free(as); free(T); free(rt); free(sq); free(phi); free(prev);
    free(cur); free(nxt); free(X); free(Y); free(q); free(rem); free(S);
    free(A296493_digits);
    A296493_digits = out;
    A296493_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > A296493_have) {
        size_t M = A296493_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 128) M = 128;
        A296493_compute(M);
    }
    return (seq_t)A296493_digits[n - 1];
}
