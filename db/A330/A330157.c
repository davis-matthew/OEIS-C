/*
 * A330157  Decimal expansion of the sum of the reciprocals of A020330.
 * https://oeis.org/A330157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=1} 1/A020330(k), A020330(k)
 *            = k*(2^L+1) with L = bit length of k. Direct sum for k < 4096;
 *            for L >= 13, the block sum S_L = Sum_{k=2^(L-1)}^{2^L-1} 1/k is
 *            taken from its asymptotic expansion S_L = log(2) + q +
 *            Sum_{j>=1} (-1)^(j-1) T_j q^(2j), where q = 2^-(L+...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define OS_FW 28
#define OS_NW 29
#define OS_TW 20
#define OS_J 40
#define OS_NDIG 200

typedef uint32_t os_u32;

static void os_add_at(os_u32 *a, uint64_t v, long pos)
{
    if (pos <= -32) return;
    if (pos < 0) { v >>= (-pos); pos = 0; }
    long idx = pos / 32; int off = (int)(pos % 32);
    uint64_t carry = v << off;
    while (carry && idx < OS_NW) {
        carry += a[idx];
        a[idx] = (os_u32)carry;
        carry >>= 32;
        idx++;
    }
}

static void os_sub_at(os_u32 *a, uint64_t v, long pos)
{
    if (pos <= -32) return;
    if (pos < 0) { v >>= (-pos); pos = 0; }
    long idx = pos / 32; int off = (int)(pos % 32);
    uint64_t sub = v << off;
    while (sub && idx < OS_NW) {
        uint64_t cur = a[idx];
        uint64_t low = sub & 0xFFFFFFFFull;
        sub >>= 32;
        if (cur < low) { cur += (1ull << 32); sub++; }
        a[idx] = (os_u32)(cur - low);
        idx++;
    }
}

static void os_add_big(os_u32 *a, const os_u32 *b)
{
    uint64_t c = 0;
    for (int i = 0; i < OS_NW; i++) {
        c += (uint64_t)a[i] + b[i];
        a[i] = (os_u32)c;
        c >>= 32;
    }
}

static void os_sub_big(os_u32 *a, const os_u32 *b)
{
    int64_t br = 0;
    for (int i = 0; i < OS_NW; i++) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - br;
        if (cur < 0) { cur += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (os_u32)cur;
    }
}

static void os_shr_copy(os_u32 *dst, const os_u32 *src, long bits)
{
    long ws = bits / 32; int bs = (int)(bits % 32);
    for (long i = 0; i < OS_NW; i++) {
        os_u32 lo = (i + ws < OS_NW) ? src[i + ws] : 0;
        os_u32 hi = (i + ws + 1 < OS_NW) ? src[i + ws + 1] : 0;
        if (bs) dst[i] = (os_u32)((lo >> bs) | (((uint64_t)hi << (32 - bs)) & 0xFFFFFFFFull));
        else dst[i] = lo;
    }
}

static void os_div_small(os_u32 *dst, const os_u32 *src, uint64_t d)
{
    uint64_t rem = 0;
    for (int i = OS_NW - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | src[i];
        dst[i] = (os_u32)(cur / d);
        rem = cur % d;
    }
}

static int os_ready = 0;
static int os_digits[OS_NDIG];

static void os_compute(void)
{
    os_u32 total[OS_NW], tmp[OS_NW], tmp2[OS_NW], ln2[OS_NW], S[OS_NW], one[OS_NW];
    static os_u32 T[OS_J + 1][OS_TW];
    int i, j, k, w;

    for (i = 0; i < OS_NW; i++) { total[i] = 0; ln2[i] = 0; one[i] = 0; }
    one[OS_FW] = 1;

    /* direct part: k < 4096 */
    for (int L = 1; L <= 12; L++) {
        for (uint64_t kk = (uint64_t)1 << (L - 1); kk < ((uint64_t)1 << L); kk++) {
            uint64_t d = kk * (((uint64_t)1 << L) + 1);
            uint64_t rem = 0;
            for (i = OS_NW - 1; i >= 0; i--) {
                uint64_t cur = (rem << 32) | (i == OS_FW ? 1u : 0u);
                tmp[i] = (os_u32)(cur / d);
                rem = cur % d;
            }
            os_add_big(total, tmp);
        }
    }

    /* ln 2 = sum 1/(k 2^k) */
    for (k = 1; k <= 32 * OS_FW + 4; k++) {
        os_shr_copy(tmp, one, k);
        os_div_small(tmp2, tmp, (uint64_t)k);
        os_add_big(ln2, tmp2);
    }

    /* tangent numbers */
    for (j = 0; j <= OS_J; j++) for (w = 0; w < OS_TW; w++) T[j][w] = 0;
    T[1][0] = 1;
    for (k = 2; k <= OS_J; k++) {
        uint64_t c = 0;
        for (w = 0; w < OS_TW; w++) {
            c += (uint64_t)T[k - 1][w] * (uint64_t)(k - 1);
            T[k][w] = (os_u32)c;
            c >>= 32;
        }
    }
    for (k = 2; k <= OS_J; k++) {
        for (j = k; j <= OS_J; j++) {
            uint64_t a = (uint64_t)(j - k), b = (uint64_t)(j - k + 2), c = 0;
            for (w = 0; w < OS_TW; w++) {
                c += a * T[j - 1][w] + b * T[j][w];
                T[j][w] = (os_u32)c;
                c >>= 32;
            }
        }
    }

    /* tail: L >= 13 */
    for (int L = 13; L <= 32 * OS_FW + 2; L++) {
        for (i = 0; i < OS_NW; i++) S[i] = ln2[i];
        os_add_at(S, 1, 32L * OS_FW - (L + 1));
        for (j = 1; j <= OS_J; j++) {
            for (w = 0; w < OS_TW; w++) {
                if (!T[j][w]) continue;
                long pos = 32L * w + 32L * OS_FW - 2L * j * (L + 1);
                if (pos >= 32L * OS_NW) continue;
                if (j & 1) os_add_at(S, T[j][w], pos);
                else os_sub_at(S, T[j][w], pos);
            }
        }
        for (i = 0; ; i++) {
            long sh = (long)L * (i + 1);
            if (sh >= 32L * OS_NW) break;
            os_shr_copy(tmp, S, sh);
            if (i & 1) os_sub_big(total, tmp);
            else os_add_big(total, tmp);
        }
    }

    /* digits */
    for (int d = 0; d < OS_NDIG; d++) {
        uint64_t carry = 0;
        for (i = 0; i < OS_NW; i++) {
            uint64_t t = (uint64_t)total[i] * 10u + carry;
            total[i] = (os_u32)t;
            carry = t >> 32;
        }
        os_digits[d] = (int)total[OS_FW];
        total[OS_FW] = 0;
    }
    os_ready = 1;
}

seq_t a(idx_t n)
{
    if (!os_ready) os_compute();
    if (n >= OS_NDIG) return (seq_t)0;
    return (seq_t)os_digits[(int)n];
}
