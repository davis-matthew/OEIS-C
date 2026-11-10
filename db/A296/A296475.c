/*
 * A296475  Decimal expansion of ratio-sum for A295949; see Comments.
 * https://oeis.org/A296475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th digit of S = Sum_{k>=1} |A295949(k)/A295949(k-1) -
 *            phi|, phi=(1+sqrt(5))/2; with d(1)=2-phi,
 *            d(k)=b(k)-(phi-1)*d(k-1), term_k = d(k)/A295949(k-1) (b =
 *            complement of A295949), computed in big fixed-point arithmetic
 *            and read off as decimal digits
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

typedef uint32_t bn_u32;
typedef uint64_t bn_u64;

static int bn_cmp(const bn_u32 *a, const bn_u32 *b, size_t W) {
    for (size_t i = W; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

static void bn_sub(bn_u32 *a, const bn_u32 *b, size_t W) {
    bn_u64 borrow = 0;
    for (size_t i = 0; i < W; i++) {
        bn_u64 v = (bn_u64)a[i] - (bn_u64)b[i] - borrow;
        a[i] = (bn_u32)v;
        borrow = (v >> 63) & 1;
    }
}

static void bn_add(bn_u32 *a, const bn_u32 *b, size_t W) {
    bn_u64 carry = 0;
    for (size_t i = 0; i < W; i++) {
        bn_u64 v = (bn_u64)a[i] + b[i] + carry;
        a[i] = (bn_u32)v;
        carry = v >> 32;
    }
}

static void bn_shl1(bn_u32 *x, size_t W, bn_u32 bit) {
    bn_u32 c = bit;
    for (size_t i = 0; i < W; i++) {
        bn_u32 nc = x[i] >> 31;
        x[i] = (x[i] << 1) | c;
        c = nc;
    }
}

static int bn_iszero(const bn_u32 *x, size_t W) {
    for (size_t i = 0; i < W; i++) if (x[i]) return 0;
    return 1;
}

/* q = num / den (den nonzero), schoolbook binary long division */
static void bn_div(const bn_u32 *num, const bn_u32 *den, bn_u32 *q, bn_u32 *rem, size_t W) {
    memset(q, 0, W * sizeof(bn_u32));
    memset(rem, 0, W * sizeof(bn_u32));
    long top = -1;
    for (size_t i = W; i > 0; i--) {
        if (num[i - 1]) {
            bn_u32 v = num[i - 1];
            int b = 31;
            while (!((v >> b) & 1)) b--;
            top = (long)((i - 1) * 32 + b);
            break;
        }
    }
    for (long i = top; i >= 0; i--) {
        bn_shl1(rem, W, (num[i / 32] >> (i % 32)) & 1u);
        if (bn_cmp(rem, den, W) >= 0) {
            bn_sub(rem, den, W);
            q[i / 32] |= (bn_u32)1 << (i % 32);
        }
    }
}

/* x = v << sh (v < 2^32) */
static void bn_set_shifted(bn_u32 *x, size_t W, bn_u64 v, size_t sh) {
    memset(x, 0, W * sizeof(bn_u32));
    size_t ls = sh / 32, bs = sh % 32;
    bn_u64 t = v << bs;
    x[ls] = (bn_u32)t;
    x[ls + 1] = (bn_u32)(t >> 32);
}

/* out = (a*b) >> B, fixed point; t has 2W+2 limbs */
static void bn_mulfix(const bn_u32 *a, const bn_u32 *b, bn_u32 *out, size_t W, size_t B, bn_u32 *t) {
    memset(t, 0, (2 * W + 2) * sizeof(bn_u32));
    for (size_t i = 0; i < W; i++) {
        if (!a[i]) continue;
        bn_u64 carry = 0;
        for (size_t j = 0; j < W; j++) {
            bn_u64 v = (bn_u64)a[i] * b[j] + t[i + j] + carry;
            t[i + j] = (bn_u32)v;
            carry = v >> 32;
        }
        size_t k = i + W;
        while (carry) {
            bn_u64 v = (bn_u64)t[k] + carry;
            t[k] = (bn_u32)v;
            carry = v >> 32;
            k++;
        }
    }
    size_t ls = B / 32, bs = B % 32;
    for (size_t i = 0; i < W; i++) {
        bn_u64 lo = t[i + ls];
        bn_u64 hi = t[i + ls + 1];
        bn_u64 v = (hi << 32) | lo;
        out[i] = (bn_u32)(v >> bs);
    }
}

static uint8_t *rs_digits = NULL;
static size_t rs_have = 0;

static void rs_compute(size_t M) {
    size_t B = (M + 25) * 10 / 3 + 64;
    size_t W = B / 32 + 6;
    size_t W2 = 2 * B / 32 + 10;

    /* phi = F(k+1)*2^B / F(k) */
    bn_u32 *phi = (bn_u32 *)calloc(W, sizeof(bn_u32));
    {
        size_t k = B * 3 / 4 + 10;
        bn_u32 *fa = (bn_u32 *)calloc(W2, sizeof(bn_u32));
        bn_u32 *fb = (bn_u32 *)calloc(W2, sizeof(bn_u32));
        bn_u32 *num = (bn_u32 *)calloc(W2, sizeof(bn_u32));
        bn_u32 *q = (bn_u32 *)calloc(W2, sizeof(bn_u32));
        bn_u32 *rm = (bn_u32 *)calloc(W2, sizeof(bn_u32));
        fb[0] = 1;
        for (size_t i = 0; i < k; i++) {
            bn_u32 *tmp = num;           /* scratch */
            memcpy(tmp, fb, W2 * sizeof(bn_u32));
            bn_add(fb, fa, W2);
            memcpy(fa, tmp, W2 * sizeof(bn_u32));
        }
        /* fa = F(k), fb = F(k+1); num = fb << B */
        {
            size_t ls = B / 32, bs = B % 32;
            memset(num, 0, W2 * sizeof(bn_u32));
            for (size_t i = 0; i + ls < W2; i++) {
                bn_u64 v = (bn_u64)fb[i] << bs;
                num[i + ls] |= (bn_u32)v;
                if (i + ls + 1 < W2) num[i + ls + 1] |= (bn_u32)(v >> 32);
            }
        }
        bn_div(num, fa, q, rm, W2);
        memcpy(phi, q, W * sizeof(bn_u32));
        free(fa); free(fb); free(num); free(q); free(rm);
    }

    bn_u32 *one = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *c = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *d = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *m = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *S = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *q = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *rm = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *ax = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *ay = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *tb = (bn_u32 *)calloc(W, sizeof(bn_u32));
    bn_u32 *tt = (bn_u32 *)calloc(2 * W + 2, sizeof(bn_u32));

    /* c = phi - 1 */
    bn_set_shifted(one, W, 1, B);
    memcpy(c, phi, W * sizeof(bn_u32));
    bn_sub(c, one, W);

    /* d(1) = 2 - phi ; term_1 = d(1)/a(0) = d(1) */
    bn_set_shifted(d, W, 2, B);
    bn_sub(d, phi, W);
    bn_add(S, d, W);

    ax[0] = 1;  /* a(0) */
    ay[0] = 2;  /* a(1) */

    uint64_t sa[80];
    size_t cnt = 2;
    sa[0] = 1; sa[1] = 2;
    uint64_t prevb = 5;

    for (size_t n = 2;; n++) {
        uint64_t b;
        if (n == 2) {
            b = 5;
        } else {
            b = prevb + 1;
            for (;;) {
                int found = 0;
                for (size_t i = 0; i < cnt; i++) if (sa[i] == b) { found = 1; break; }
                if (!found) break;
                b++;
            }
        }
        prevb = b;
        if (cnt == n && cnt < 72) {
            sa[cnt] = sa[cnt - 1] + sa[cnt - 2] + b;
            cnt++;
        }
        /* d(n) = b*2^B - c*d(n-1) */
        bn_mulfix(c, d, m, W, B, tt);
        bn_set_shifted(tb, W, b, B);
        bn_sub(tb, m, W);
        memcpy(d, tb, W * sizeof(bn_u32));
        /* term = d / a(n-1) */
        bn_div(d, ay, q, rm, W);
        if (bn_iszero(q, W)) break;
        bn_add(S, q, W);
        /* a(n) = a(n-1) + a(n-2) + b(n) */
        {
            bn_u32 bb[2];
            bb[0] = (bn_u32)b; bb[1] = (bn_u32)(b >> 32);
            bn_add(ax, ay, W);
            bn_u64 carry = bb[0];
            for (size_t i = 0; i < W && carry; i++) {
                bn_u64 v = (bn_u64)ax[i] + carry;
                ax[i] = (bn_u32)v;
                carry = v >> 32;
            }
            bn_u32 *tmp = ax; ax = ay; ay = tmp;  /* ay = a(n), ax = a(n-1) (becomes a(n-2) next after swap semantic) */
            /* after swap: ay=a(n), ax=a(n-1)  -> correct for next step */
        }
    }

    /* extract digits */
    uint8_t *out = (uint8_t *)malloc(M);
    {
        size_t ls = B / 32, bs = B % 32;
        for (size_t i = 0; i < M; i++) {
            bn_u64 v = ((bn_u64)S[ls + 1] << 32) | S[ls];
            bn_u32 dg = (bn_u32)((v >> bs) & 0xFF);
            out[i] = (uint8_t)dg;
            /* clear bits >= B */
            S[ls] &= (bs ? (((bn_u32)1 << bs) - 1) : 0u);
            for (size_t j = ls + 1; j < W; j++) S[j] = 0;
            /* multiply fraction by 10 */
            bn_u64 carry = 0;
            for (size_t j = 0; j < W; j++) {
                bn_u64 w = (bn_u64)S[j] * 10 + carry;
                S[j] = (bn_u32)w;
                carry = w >> 32;
            }
        }
    }

    free(phi); free(one); free(c); free(d); free(m); free(S);
    free(q); free(rm); free(ax); free(ay); free(tb); free(tt);
    free(rs_digits);
    rs_digits = out;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t M = rs_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 64) M = 64;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
