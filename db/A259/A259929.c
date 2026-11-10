/*
 * A259929  Value of DIS (Decimal Integer Series) constant based on the
 *          sequence of cubes.
 * https://oeis.org/A259929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   Equals Sum_{n>=1} c/100^A055642(c) where c = A000578(n) = n^3.
 *            Computed digit by digit: for each digit length d, the cubes with
 *            d digits are k^3 for k in (m_{d-1}, m_d], where m_d =
 *            floor(cbrt(10^d - 1)). Their sum is S_d = T(m_d)^2 -
 *            T(m_{d-1})^2 with T(m) = m(m+1)/2. S_d / 10^(2d) is added
 *            exac...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A259929_BASE 1000000000u

typedef struct { uint32_t *v; int n; } A259929_Big;
static int A259929_cap;

static A259929_Big A259929_bnew(void) {
    A259929_Big b;
    b.v = (uint32_t *)calloc((size_t)A259929_cap, sizeof(uint32_t));
    b.n = 0;
    return b;
}
static void A259929_trim(A259929_Big *a) {
    while (a->n > 0 && a->v[a->n - 1] == 0) a->n--;
}
static void A259929_copy(A259929_Big *d, const A259929_Big *s) {
    for (int i = 0; i < s->n; i++) d->v[i] = s->v[i];
    d->n = s->n;
}
static int A259929_cmp(const A259929_Big *a, const A259929_Big *b) {
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (int i = a->n - 1; i >= 0; i--)
        if (a->v[i] != b->v[i]) return a->v[i] < b->v[i] ? -1 : 1;
    return 0;
}
static void A259929_add(A259929_Big *r, const A259929_Big *a, const A259929_Big *b) {
    int n = a->n > b->n ? a->n : b->n;
    uint64_t carry = 0;
    for (int i = 0; i < n; i++) {
        uint64_t s = carry;
        if (i < a->n) s += a->v[i];
        if (i < b->n) s += b->v[i];
        r->v[i] = (uint32_t)(s % A259929_BASE);
        carry = s / A259929_BASE;
    }
    r->n = n;
    if (carry) r->v[r->n++] = (uint32_t)carry;
}
static void A259929_sub(A259929_Big *r, const A259929_Big *a, const A259929_Big *b) {
    int64_t borrow = 0;
    for (int i = 0; i < a->n; i++) {
        int64_t s = (int64_t)a->v[i] - borrow - (i < b->n ? (int64_t)b->v[i] : 0);
        if (s < 0) { s += A259929_BASE; borrow = 1; } else borrow = 0;
        r->v[i] = (uint32_t)s;
    }
    r->n = a->n;
    A259929_trim(r);
}
static void A259929_dec(A259929_Big *a) {
    for (int i = 0; i < a->n; i++) {
        if (a->v[i] > 0) { a->v[i]--; break; }
        a->v[i] = A259929_BASE - 1;
    }
    A259929_trim(a);
}
static void A259929_mul(A259929_Big *r, const A259929_Big *a, const A259929_Big *b) {
    if (a->n == 0 || b->n == 0) { r->n = 0; return; }
    int tot = a->n + b->n;
    for (int i = 0; i < tot; i++) r->v[i] = 0;
    for (int i = 0; i < a->n; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < b->n; j++) {
            uint64_t cur = (uint64_t)r->v[i + j] + (uint64_t)a->v[i] * b->v[j] + carry;
            r->v[i + j] = (uint32_t)(cur % A259929_BASE);
            carry = cur / A259929_BASE;
        }
        r->v[i + b->n] += (uint32_t)carry;
    }
    r->n = tot;
    A259929_trim(r);
}
static void A259929_mul_small(A259929_Big *a, uint32_t k) {
    uint64_t carry = 0;
    for (int i = 0; i < a->n; i++) {
        uint64_t cur = (uint64_t)a->v[i] * k + carry;
        a->v[i] = (uint32_t)(cur % A259929_BASE);
        carry = cur / A259929_BASE;
    }
    while (carry) { a->v[a->n++] = (uint32_t)(carry % A259929_BASE); carry /= A259929_BASE; }
}
static void A259929_add_small(A259929_Big *a, uint32_t k) {
    uint64_t carry = k;
    for (int i = 0; i < a->n && carry; i++) {
        uint64_t cur = (uint64_t)a->v[i] + carry;
        a->v[i] = (uint32_t)(cur % A259929_BASE);
        carry = cur / A259929_BASE;
    }
    if (carry) a->v[a->n++] = (uint32_t)carry;
}
static void A259929_div2(A259929_Big *a) {
    uint64_t rem = 0;
    for (int i = a->n - 1; i >= 0; i--) {
        uint64_t cur = rem * A259929_BASE + a->v[i];
        a->v[i] = (uint32_t)(cur / 2);
        rem = cur % 2;
    }
    A259929_trim(a);
}

static unsigned char *A259929_tab = 0;
static idx_t A259929_len = 0;

static void A259929_build(idx_t L) {
    int D = (int)((3 * (L + 1)) / 2 + 6);
    A259929_cap = D / 6 + 12;
    int *sum = (int *)calloc((size_t)L + 1, sizeof(int));
    A259929_Big P = A259929_bnew(), prev = A259929_bnew(), lo = A259929_bnew(),
                hi = A259929_bnew(), mid = A259929_bnew(), t1 = A259929_bnew(),
                t2 = A259929_bnew(), T = A259929_bnew(), Fp = A259929_bnew(),
                Fc = A259929_bnew(), S = A259929_bnew();
    static const uint32_t p10[9] = {1,10,100,1000,10000,100000,1000000,10000000,100000000};
    for (int d = 1; d <= D; d++) {
        int z = d / 9;
        for (int i = 0; i < z; i++) P.v[i] = 0;
        P.v[z] = p10[d % 9];
        P.n = z + 1;
        /* largest m with m^3 < 10^d; it lies in [2*prev, 3*prev+3] */
        A259929_copy(&lo, &prev); A259929_mul_small(&lo, 2);
        A259929_copy(&hi, &prev); A259929_mul_small(&hi, 3); A259929_add_small(&hi, 3);
        while (A259929_cmp(&lo, &hi) < 0) {
            A259929_add(&mid, &lo, &hi);
            A259929_add_small(&mid, 1);
            A259929_div2(&mid);
            A259929_mul(&t1, &mid, &mid);
            A259929_mul(&t2, &t1, &mid);
            if (A259929_cmp(&t2, &P) < 0) A259929_copy(&lo, &mid);
            else { A259929_copy(&hi, &mid); A259929_dec(&hi); }
        }
        A259929_copy(&prev, &lo);
        A259929_copy(&t1, &prev);
        A259929_add_small(&t1, 1);
        A259929_mul(&T, &prev, &t1);
        A259929_div2(&T);
        A259929_mul(&Fc, &T, &T);
        A259929_sub(&S, &Fc, &Fp);
        A259929_copy(&Fp, &Fc);
        /* add S / 10^(2d) into digit sums; last digit of S at position 2d */
        long pos = 2L * d;
        for (int i = 0; i < S.n && pos >= 1; i++) {
            uint32_t limb = S.v[i];
            for (int k = 0; k < 9 && pos >= 1; k++) {
                if (pos <= (long)L) sum[pos - 1] += (int)(limb % 10);
                limb /= 10;
                pos--;
            }
        }
    }
    for (long i = (long)L - 1; i >= 1; i--) {
        sum[i - 1] += sum[i] / 10;
        sum[i] %= 10;
    }
    sum[0] %= 10;
    free(A259929_tab);
    A259929_tab = (unsigned char *)malloc((size_t)L);
    for (idx_t i = 0; i < L; i++) A259929_tab[i] = (unsigned char)sum[i];
    A259929_len = L - 20;
    free(sum);
    free(P.v); free(prev.v); free(lo.v); free(hi.v); free(mid.v); free(t1.v);
    free(t2.v); free(T.v); free(Fp.v); free(Fc.v); free(S.v);
}

seq_t a(idx_t n)
{
    if (n >= A259929_len) {
        idx_t L = n + 21;
        L = (L + 63) / 64 * 64;
        A259929_build(L);
    }
    return (seq_t)A259929_tab[n];
}
