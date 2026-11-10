/*
 * A258621  Decimal expansion of sum(1/A030450).
 * https://oeis.org/A258621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n-1) * Sum_{k>=0} 1/A030450(k)) mod 10, with
 *            A030450(0) = 1, A030450(k) = (k*A030450(k-1))^2; computed with
 *            exact multiprecision fixed-point arithmetic (12 guard digits)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 106 terms through n = 106 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t A258621_trim(const uint32_t *x, size_t l)
{
    while (l > 0 && x[l - 1] == 0) l--;
    return l;
}

static void A258621_mul_small(uint32_t *x, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i)
    {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) x[(*len)++] = (uint32_t)carry;
    *len = A258621_trim(x, *len);
}

static uint32_t A258621_div_small(uint32_t *x, size_t *len, uint32_t d)
{
    uint64_t r = 0;
    for (size_t i = *len; i-- > 0;)
    {
        uint64_t t = (r << 32) | x[i];
        x[i] = (uint32_t)(t / d);
        r = t % d;
    }
    *len = A258621_trim(x, *len);
    return (uint32_t)r;
}

static int A258621_cmp(const uint32_t *a, size_t la, const uint32_t *b, size_t lb)
{
    la = A258621_trim(a, la);
    lb = A258621_trim(b, lb);
    if (la != lb) return la < lb ? -1 : 1;
    for (size_t i = la; i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

static size_t A258621_mul(uint32_t *out, const uint32_t *a, size_t la,
                          const uint32_t *b, size_t lb)
{
    memset(out, 0, (la + lb) * sizeof(uint32_t));
    for (size_t i = 0; i < la; ++i)
    {
        uint64_t carry = 0;
        for (size_t j = 0; j < lb; ++j)
        {
            uint64_t t = (uint64_t)a[i] * b[j] + out[i + j] + carry;
            out[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        out[i + lb] = (uint32_t)carry;
    }
    return A258621_trim(out, la + lb);
}

/* q = floor(u / v), u has m limbs, v has nn limbs, m >= nn >= 1, v[nn-1] != 0.
   un needs m+1 limbs, vn needs nn limbs; q needs m-nn+1 limbs. */
static void A258621_divmnu(uint32_t *q, const uint32_t *u, size_t m,
                           const uint32_t *v, size_t nn,
                           uint32_t *un, uint32_t *vn)
{
    const uint64_t b = (uint64_t)1 << 32;
    if (nn == 1)
    {
        uint64_t k = 0;
        for (size_t j = m; j-- > 0;)
        {
            uint64_t t = (k << 32) | u[j];
            q[j] = (uint32_t)(t / v[0]);
            k = t % v[0];
        }
        return;
    }
    int s = 0;
    {
        uint32_t top = v[nn - 1];
        while (!(top & 0x80000000u)) { top <<= 1; s++; }
    }
    for (size_t i = nn - 1; i > 0; --i)
        vn[i] = (v[i] << s) | (s ? (uint32_t)((uint64_t)v[i - 1] >> (32 - s)) : 0);
    vn[0] = v[0] << s;
    un[m] = s ? (uint32_t)((uint64_t)u[m - 1] >> (32 - s)) : 0;
    for (size_t i = m - 1; i > 0; --i)
        un[i] = (u[i] << s) | (s ? (uint32_t)((uint64_t)u[i - 1] >> (32 - s)) : 0);
    un[0] = u[0] << s;

    for (long long j = (long long)(m - nn); j >= 0; --j)
    {
        uint64_t num = ((uint64_t)un[j + nn] << 32) | un[j + nn - 1];
        uint64_t qhat = num / vn[nn - 1];
        uint64_t rhat = num % vn[nn - 1];
        while (qhat >= b ||
               qhat * vn[nn - 2] > ((rhat << 32) | un[j + nn - 2]))
        {
            qhat--;
            rhat += vn[nn - 1];
            if (rhat >= b) break;
        }
        int64_t k = 0, t;
        for (size_t i = 0; i < nn; ++i)
        {
            uint64_t p = qhat * vn[i];
            t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFu);
            un[i + j] = (uint32_t)t;
            k = (int64_t)(p >> 32) - (t >> 32);
        }
        t = (int64_t)un[j + nn] - k;
        un[j + nn] = (uint32_t)t;
        q[j] = (uint32_t)qhat;
        if (t < 0)
        {
            q[j]--;
            uint64_t c = 0;
            for (size_t i = 0; i < nn; ++i)
            {
                uint64_t tt = (uint64_t)un[i + j] + vn[i] + c;
                un[i + j] = (uint32_t)tt;
                c = tt >> 32;
            }
            un[j + nn] = (uint32_t)((uint64_t)un[j + nn] + c);
        }
    }
}

seq_t a(idx_t n)
{
    const idx_t g = 12;
    idx_t M = n - 1 + g;
    size_t ln = (size_t)((M * 3322) / 1000 / 32 + 4);
    size_t cap = 2 * ln + 8;

    uint32_t *num = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *cur = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *un = (uint32_t *)calloc(cap + 1, sizeof(uint32_t));
    uint32_t *vn = (uint32_t *)calloc(cap, sizeof(uint32_t));

    /* num = 10^M */
    size_t lnum = 1;
    num[0] = 1;
    {
        idx_t e = M;
        while (e >= 9) { A258621_mul_small(num, &lnum, 1000000000u); e -= 9; }
        uint32_t p = 1;
        while (e > 0) { p *= 10; e--; }
        if (p > 1) A258621_mul_small(num, &lnum, p);
    }

    /* S = 10^M / A030450(0) */
    memcpy(S, num, lnum * sizeof(uint32_t));
    size_t lS = lnum;

    size_t lcur = 1;
    cur[0] = 1;
    for (uint32_t k = 1;; ++k)
    {
        memcpy(tmp, cur, lcur * sizeof(uint32_t));
        size_t ltmp = lcur;
        A258621_mul_small(tmp, &ltmp, k);
        lcur = A258621_mul(cur, tmp, ltmp, tmp, ltmp);
        if (A258621_cmp(cur, lcur, num, lnum) > 0) break;

        size_t lq = lnum - lcur + 1;
        memset(q, 0, lq * sizeof(uint32_t));
        A258621_divmnu(q, num, lnum, cur, lcur, un, vn);

        /* S += q */
        uint64_t c = 0;
        size_t L = lS > lq ? lS : lq;
        for (size_t i = 0; i < L; ++i)
        {
            uint64_t t = (uint64_t)(i < lS ? S[i] : 0) + (i < lq ? q[i] : 0) + c;
            S[i] = (uint32_t)t;
            c = t >> 32;
        }
        if (c) S[L++] = (uint32_t)c;
        lS = A258621_trim(S, L);
    }

    A258621_div_small(S, &lS, 1000000u);
    A258621_div_small(S, &lS, 1000000u);
    uint32_t d = A258621_div_small(S, &lS, 10u);

    free(num); free(S); free(cur); free(tmp); free(q); free(un); free(vn);
    return (seq_t)d;
}
