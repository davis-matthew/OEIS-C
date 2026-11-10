/*
 * A372551  Decimal expansion of Sum_{k >= 1}
 *          (32*A102683(k)-A316869(k)/32)/2^k.
 * https://oeis.org/A372551
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit of S = Sum_{k>=1} (32*A102683(k) -
 *            A316869(k)/32)/2^k at decimal position n+3 after the point,
 *            computed by exact fixed-point big-integer arithmetic
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8574 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A372551_dig = 0;
static long long A372551_cap = 0;

static void A372551_addat(uint32_t *arr, long long p, uint64_t w)
{
    long long i = p / 32;
    int off = (int)(p % 32);
    uint64_t v = w << off;
    uint64_t s = (uint64_t)arr[i] + (v & 0xFFFFFFFFULL);
    arr[i] = (uint32_t)s;
    uint64_t carry = (s >> 32) + (v >> 32);
    ++i;
    while (carry)
    {
        s = (uint64_t)arr[i] + carry;
        arr[i] = (uint32_t)s;
        carry = s >> 32;
        ++i;
    }
}

static void A372551_compute(long long M)
{
    long long K = (M + 30) * 10 / 3 + 64;
    long long B = K + 5;
    long long W = (B + 31) / 32;
    uint32_t *P = (uint32_t *)calloc((size_t)W + 4, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc((size_t)W + 4, sizeof(uint32_t));
    uint32_t *F = (uint32_t *)calloc((size_t)W + 4, sizeof(uint32_t));
    unsigned char *d = (unsigned char *)malloc((size_t)M + 1);
    long long k, i;
    for (k = 1; k <= K; k++)
    {
        int c9 = 0, c8 = 0;
        long long m = k;
        while (m > 0)
        {
            int r = (int)(m % 10);
            if (r == 9) ++c9;
            else if (r == 8) ++c8;
            m /= 10;
        }
        if (c9) A372551_addat(P, K - k, (uint64_t)c9 * 1024u);
        if (c8) A372551_addat(N, K - k, (uint64_t)c8);
    }
    /* T = P - N */
    {
        uint64_t borrow = 0;
        for (i = 0; i < W + 2; i++)
        {
            uint64_t a = P[i], b = (uint64_t)N[i] + borrow;
            if (a >= b) { P[i] = (uint32_t)(a - b); borrow = 0; }
            else { P[i] = (uint32_t)(a + 4294967296ULL - b); borrow = 1; }
        }
    }
    {
        int s = (int)(32 * W - B);
        for (i = W - 1; i >= 0; i--)
        {
            uint32_t v = P[i] << s;
            if (s > 0 && i > 0) v |= P[i - 1] >> (32 - s);
            F[i] = v;
        }
    }
    for (long long m = 0; m < M; m++)
    {
        uint64_t carry = 0;
        for (i = 0; i < W; i++)
        {
            uint64_t t = (uint64_t)F[i] * 10u + carry;
            F[i] = (uint32_t)t;
            carry = t >> 32;
        }
        d[m] = (unsigned char)carry;
    }
    free(P); free(N); free(F);
    if (A372551_dig) free(A372551_dig);
    A372551_dig = d;
    A372551_cap = M;
}

seq_t a(idx_t n)
{
    long long need = (long long)n + 3;
    if (need < 1) return 0;
    if (A372551_cap < need)
    {
        long long nm = need * 3 / 2;
        if (nm < 2048) nm = 2048;
        A372551_compute(nm);
    }
    return (seq_t)A372551_dig[need - 1];
}
