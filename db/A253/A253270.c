/*
 * A253270  Decimal expansion of a constant related to A253268.
 * https://oeis.org/A253270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   A253270 = Product_{k>=1} (1 - (-1/phi^2)^k)^k = exp(-Sum_{N>=1}
 *            sigma_2(N)/N * (-q)^N), q = phi^-2, phi = (1+sqrt(5))/2; a(n) =
 *            n-th decimal digit of this constant, computed in base-1e9 fixed
 *            point with q^N from q^(N+1) = 3q^N - q^(N-1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A253270_B 1000000000ULL

static void A253270_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; ++i) {
        uint64_t cur = (uint64_t)a[i] + b[i] + c;
        if (cur >= A253270_B) { cur -= A253270_B; c = 1; } else c = 0;
        a[i] = (uint32_t)cur;
    }
}

static void A253270_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    for (size_t i = 0; i < L; ++i) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - br;
        if (cur < 0) { cur += (int64_t)A253270_B; br = 1; } else br = 0;
        a[i] = (uint32_t)cur;
    }
}

static void A253270_mulsmall(uint32_t *a, uint64_t m, size_t L) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; ++i) {
        uint64_t cur = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)(cur % A253270_B);
        c = cur / A253270_B;
    }
}

static void A253270_divsmall(uint32_t *a, uint64_t d, size_t L) {
    uint64_t rem = 0;
    for (size_t i = L; i-- > 0;) {
        uint64_t cur = rem * A253270_B + a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static bool A253270_iszero(const uint32_t *a, size_t L) {
    for (size_t i = 0; i < L; ++i) if (a[i]) return false;
    return true;
}

/* fixed point: L = F+1 limbs, limb F is the integer part */
static void A253270_mulfix(uint32_t *out, const uint32_t *a, const uint32_t *b,
                           size_t L, uint32_t *prod) {
    size_t F = L - 1;
    memset(prod, 0, 2 * L * sizeof(uint32_t));
    for (size_t i = 0; i < L; ++i) {
        if (a[i] == 0) continue;
        uint64_t c = 0;
        for (size_t j = 0; j < L; ++j) {
            uint64_t cur = (uint64_t)prod[i + j] + (uint64_t)a[i] * b[j] + c;
            prod[i + j] = (uint32_t)(cur % A253270_B);
            c = cur / A253270_B;
        }
        prod[i + L] = (uint32_t)((uint64_t)prod[i + L] + c);
    }
    for (size_t i = 0; i < L; ++i) out[i] = prod[F + i];
}

static uint64_t A253270_sigma2(uint64_t n) {
    uint64_t s = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            s += d * d;
            uint64_t e = n / d;
            if (e != d) s += e * e;
        }
    }
    return s;
}

static unsigned char *A253270_dig = 0;
static size_t A253270_have = 0;

static void A253270_compute(size_t target) {
    size_t T = target + 30;
    size_t W = (21 * T) / 10 + 40;
    size_t F = W / 9 + 2;
    size_t L = F + 1;
    size_t Nmax = (T * 12) / 5 + 8;

    uint32_t *prod = (uint32_t *)malloc(2 * L * sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *pp = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *pc = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *pn = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tm = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *so = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *se = (uint32_t *)calloc(L, sizeof(uint32_t));

    /* r = 1/sqrt(5) by Newton */
    r[F - 1] = 447213595U;
    if (F >= 2) r[F - 2] = 499957900U;
    size_t p = 15;
    int iters = 2;
    while (p < W + 20) { p *= 2; ++iters; }
    for (int it = 0; it < iters; ++it) {
        A253270_mulfix(t, r, r, L, prod);
        A253270_mulsmall(t, 5, L);
        memset(u, 0, L * sizeof(uint32_t));
        u[F] = 3;
        A253270_sub(u, t, L);
        A253270_mulfix(r, r, u, L, prod);
        A253270_divsmall(r, 2, L);
    }
    /* q = (3 - 5r)/2 */
    memcpy(t, r, L * sizeof(uint32_t));
    A253270_mulsmall(t, 5, L);
    memset(q, 0, L * sizeof(uint32_t));
    q[F] = 3;
    A253270_sub(q, t, L);
    A253270_divsmall(q, 2, L);

    pp[F] = 1;
    memcpy(pc, q, L * sizeof(uint32_t));
    for (size_t N = 1; N <= Nmax; ++N) {
        memcpy(tm, pc, L * sizeof(uint32_t));
        A253270_mulsmall(tm, A253270_sigma2((uint64_t)N), L);
        A253270_divsmall(tm, (uint64_t)N, L);
        if (N & 1) A253270_add(so, tm, L); else A253270_add(se, tm, L);
        memcpy(pn, pc, L * sizeof(uint32_t));
        A253270_mulsmall(pn, 3, L);
        A253270_sub(pn, pp, L);
        uint32_t *tmp = pp; pp = pc; pc = pn; pn = tmp;
    }
    /* y = so - se = ln C */
    A253270_sub(so, se, L);
    uint32_t *y = so;
    A253270_divsmall(y, 65536, L);
    A253270_divsmall(y, 65536, L);

    uint32_t *sum = se;
    memcpy(sum, y, L * sizeof(uint32_t));
    sum[F] += 1;
    memcpy(tm, y, L * sizeof(uint32_t));
    for (uint64_t j = 2;; ++j) {
        A253270_mulfix(tm, tm, y, L, prod);
        A253270_divsmall(tm, j, L);
        if (A253270_iszero(tm, L)) break;
        A253270_add(sum, tm, L);
    }
    for (int s = 0; s < 32; ++s) A253270_mulfix(sum, sum, sum, L, prod);

    unsigned char *d = (unsigned char *)malloc(target + 1);
    d[0] = 0;
    for (size_t n = 1; n <= target; ++n) {
        size_t m = n - 1;
        if (m == 0) {
            d[n] = (unsigned char)(sum[F] % 10);
        } else {
            size_t j = (m - 1) / 9;
            size_t rr = (m - 1) % 9;
            uint32_t limb = sum[F - 1 - j];
            uint32_t pw = 1;
            for (size_t k = 0; k < 8 - rr; ++k) pw *= 10;
            d[n] = (unsigned char)((limb / pw) % 10);
        }
    }
    if (A253270_dig) free(A253270_dig);
    A253270_dig = d;
    A253270_have = target;

    free(prod); free(r); free(t); free(u); free(q); free(pp); free(pc);
    free(pn); free(tm); free(so); free(se);
}

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;
    if (nn > A253270_have) {
        size_t target = nn;
        if (target < 2 * A253270_have) target = 2 * A253270_have;
        if (target < 64) target = 64;
        A253270_compute(target);
    }
    return (seq_t)A253270_dig[nn];
}
