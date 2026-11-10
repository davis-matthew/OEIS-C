/*
 * A296491  Decimal expansion of ratio-sum for A294170; see Comments.
 * https://oeis.org/A296491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th digit of S = Sum_{k>=1} |A294170(k)/A294170(k-1) -
 *            phi|, phi = (1+sqrt(5))/2; A294170: a(k) =
 *            a(k-1)+a(k-2)+b(k)+2k, a(0)=1, a(1)=2, b(0..2)=3,4,5, a/b
 *            complementary increasing. Computed with own multi-limb
 *            fixed-point arithmetic (summing terms until they vanish)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t rs_u32;
typedef uint64_t rs_u64;

static size_t rs_trim(const rs_u32 *x, size_t n) {
    while (n && !x[n - 1]) n--;
    return n;
}

static int rs_cmp(const rs_u32 *a, const rs_u32 *b, size_t n) {
    for (size_t i = n; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

static void rs_sub(rs_u32 *a, const rs_u32 *b, size_t n) {
    rs_u64 borrow = 0;
    for (size_t i = 0; i < n; i++) {
        rs_u64 v = (rs_u64)a[i] - b[i] - borrow;
        a[i] = (rs_u32)v;
        borrow = (v >> 63) & 1;
    }
}

/* out[0..Wf] = num/den as fixed point with Wf fractional limbs (out[Wf] integer part) */
static void rs_ratio(const rs_u32 *num, size_t nl, const rs_u32 *den, size_t dl,
                     rs_u32 *out, size_t Wf) {
    size_t n = dl + 1;
    rs_u32 *r = (rs_u32 *)calloc(n + 1, sizeof(rs_u32));
    rs_u32 *dd = (rs_u32 *)calloc(n + 1, sizeof(rs_u32));
    memcpy(dd, den, dl * sizeof(rs_u32));
    memcpy(r, num, nl * sizeof(rs_u32));
    rs_u32 ip = 0;
    while (rs_cmp(r, dd, n) >= 0) { rs_sub(r, dd, n); ip++; }
    out[Wf] = ip;
    for (size_t i = Wf; i > 0; i--) {
        rs_u32 w = 0;
        for (int bit = 31; bit >= 0; bit--) {
            rs_u32 carry = 0;
            for (size_t j = 0; j < n; j++) {
                rs_u32 nc = r[j] >> 31;
                r[j] = (r[j] << 1) | carry;
                carry = nc;
            }
            if (rs_cmp(r, dd, n) >= 0) { rs_sub(r, dd, n); w |= (rs_u32)1 << bit; }
        }
        out[i - 1] = w;
    }
    free(r);
    free(dd);
}

/* r = x + y (lengths lx, ly); returns trimmed length; r has room for max+1 limbs */
static size_t rs_add(rs_u32 *r, const rs_u32 *x, size_t lx, const rs_u32 *y, size_t ly) {
    size_t m = lx > ly ? lx : ly;
    rs_u64 c = 0;
    for (size_t i = 0; i < m; i++) {
        rs_u64 s = c + (i < lx ? x[i] : 0) + (i < ly ? y[i] : 0);
        r[i] = (rs_u32)s;
        c = s >> 32;
    }
    r[m] = (rs_u32)c;
    return rs_trim(r, m + 1);
}

static unsigned char *rs_digits = NULL;
static size_t rs_have = 0;

static void rs_compute(size_t M) {
    size_t bits = M * 10 / 3 + 192;
    size_t Wf = bits / 32 + 2;
    size_t cap = 2 * Wf + 16;
    size_t S = 32 * Wf;
    (void)S;

    rs_u32 *f0 = (rs_u32 *)calloc(cap, 4);
    rs_u32 *f1 = (rs_u32 *)calloc(cap, 4);
    rs_u32 *f2 = (rs_u32 *)calloc(cap, 4);
    size_t l0 = 0, l1 = 1;
    f1[0] = 1;
    while (l0 < Wf / 2 + 4) {
        size_t l2 = rs_add(f2, f0, l0, f1, l1);
        rs_u32 *t = f0; f0 = f1; f1 = f2; f2 = t;
        l0 = l1; l1 = l2;
    }
    /* now f0 = F(m), f1 = F(m+1) */
    rs_u32 *phi = (rs_u32 *)calloc(Wf + 1, 4);
    rs_ratio(f1, l1, f0, l0, phi, Wf);
    free(f0); free(f1); free(f2);

    rs_u32 *A0 = (rs_u32 *)calloc(cap, 4);
    rs_u32 *A1 = (rs_u32 *)calloc(cap, 4);
    rs_u32 *A2 = (rs_u32 *)calloc(cap, 4);
    size_t maxK = 64 * cap + 64;
    rs_u64 *sm = (rs_u64 *)malloc(maxK * sizeof(rs_u64));
    rs_u32 *R = (rs_u32 *)calloc(Wf + 1, 4);
    rs_u32 *sum = (rs_u32 *)calloc(Wf + 1, 4);

    A0[0] = 1; size_t la0 = 1;
    A1[0] = 2; size_t la1 = 1;
    sm[0] = 1; sm[1] = 2;
    rs_u64 last_b = 4;
    size_t p = 0;
    int small_cnt = 0;

    for (size_t k = 1; k < maxK; k++) {
        if (k >= 2) {
            rs_u64 cand = last_b + 1;
            for (;;) {
                while (p < k && sm[p] < cand) p++;
                if (p < k && sm[p] == cand) cand++;
                else break;
            }
            last_b = cand;
            rs_u64 add = cand + 2 * (rs_u64)k;
            rs_u32 *tmp = A2;
            size_t l2 = rs_add(tmp, A1, la1, A0, la0);
            rs_u32 ad[2];
            ad[0] = (rs_u32)add; ad[1] = (rs_u32)(add >> 32);
            /* add small value in place */
            rs_u64 c = 0;
            size_t i;
            for (i = 0; i < 2 || c; i++) {
                if (i >= cap - 1) break;
                rs_u64 s = c + (i < l2 ? tmp[i] : 0) + (i < 2 ? ad[i] : 0);
                tmp[i] = (rs_u32)s;
                c = s >> 32;
                if (i + 1 > l2 && i >= 1 && !c) { i++; break; }
            }
            if (i > l2) l2 = i;
            l2 = rs_trim(tmp, l2);
            /* store small value */
            if (l2 <= 2) {
                rs_u64 v = 0;
                if (l2 > 0) v = tmp[0];
                if (l2 > 1) v |= (rs_u64)tmp[1] << 32;
                sm[k] = v;
            } else sm[k] = UINT64_MAX;
            /* rotate */
            rs_u32 *old = A0;
            A0 = A1; la0 = la1;
            A1 = A2; la1 = l2;
            A2 = old;
        } else {
            /* k == 1: a(1)/a(0) with A0=1, A1=2 already set; swap roles for ratio */
        }
        if (la1 >= cap - 4) break;
        rs_ratio(A1, la1, A0, la0, R, Wf);
        /* |R - phi| */
        rs_u32 *big = R, *lit = phi;
        if (rs_cmp(R, phi, Wf + 1) < 0) { big = phi; lit = R; }
        rs_u32 *df = (rs_u32 *)malloc((Wf + 1) * 4);
        memcpy(df, big, (Wf + 1) * 4);
        rs_sub(df, lit, Wf + 1);
        /* sum += df */
        rs_u64 c = 0;
        for (size_t i = 0; i <= Wf; i++) {
            rs_u64 s = (rs_u64)sum[i] + df[i] + c;
            sum[i] = (rs_u32)s;
            c = s >> 32;
        }
        int zero = 1;
        for (size_t i = 2; i <= Wf; i++) if (df[i]) { zero = 0; break; }
        free(df);
        if (zero) { if (++small_cnt >= 20) break; }
        else small_cnt = 0;
    }

    unsigned char *out = (unsigned char *)malloc(M);
    out[0] = (unsigned char)sum[Wf];
    rs_u32 *fr = (rs_u32 *)malloc(Wf * 4);
    memcpy(fr, sum, Wf * 4);
    for (size_t i = 1; i < M; i++) {
        rs_u64 carry = 0;
        for (size_t j = 0; j < Wf; j++) {
            rs_u64 v = (rs_u64)fr[j] * 10 + carry;
            fr[j] = (rs_u32)v;
            carry = v >> 32;
        }
        out[i] = (unsigned char)carry;
    }
    free(fr);
    free(A0); free(A1); free(A2); free(sm); free(R); free(sum); free(phi);
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
