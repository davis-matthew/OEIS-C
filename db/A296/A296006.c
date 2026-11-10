/*
 * A296006  Decimal expansion of the limiting ratio of terms in A296005.
 * https://oeis.org/A296006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of lim_{k->oo} A296005(k)/A296005(k-1)
 *            = 3.114986447390302...; computed by iterating the complementary
 *            equation a(0)=2, a(1)=3, b(0)=1, a(m)=Sum_{k=0..m-1}
 *            a(k)*b(m-1-k), with exact big integers, then long division
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define L296006_N 1000
#define L296006_LIMBS 64
#define L296006_DIGITS 150

static uint32_t L296006_A[L296006_N + 1][L296006_LIMBS];
static uint32_t L296006_B[L296006_N + 2];
static unsigned char L296006_dig[L296006_DIGITS];
static bool L296006_done = false;

static int L296006_cmp(const uint32_t *x, const uint32_t *y)
{
    for (int j = L296006_LIMBS - 1; j >= 0; --j) {
        if (x[j] != y[j]) return x[j] < y[j] ? -1 : 1;
    }
    return 0;
}

static void L296006_sub(uint32_t *x, const uint32_t *y)
{
    int64_t borrow = 0;
    for (int j = 0; j < L296006_LIMBS; ++j) {
        int64_t t = (int64_t)x[j] - (int64_t)y[j] - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x[j] = (uint32_t)t;
    }
}

static void L296006_mul10(uint32_t *x)
{
    uint64_t c = 0;
    for (int j = 0; j < L296006_LIMBS; ++j) {
        uint64_t t = (uint64_t)x[j] * 10 + c;
        x[j] = (uint32_t)t;
        c = t >> 32;
    }
}

/* small value of A[k], or UINT64_MAX if large */
static uint64_t L296006_small(int k)
{
    for (int j = 2; j < L296006_LIMBS; ++j)
        if (L296006_A[k][j]) return UINT64_MAX;
    return ((uint64_t)L296006_A[k][1] << 32) | L296006_A[k][0];
}

static void L296006_init(void)
{
    if (L296006_done) return;
    L296006_done = true;
    int N = L296006_N;
    L296006_A[0][0] = 2;
    L296006_A[1][0] = 3;
    L296006_B[0] = 1;
    int blen = 1;
    uint64_t next_cand = 2;
    for (int m = 2; m <= N; ++m) {
        while (blen < m) {
            uint64_t cand = next_cand;
            for (;;) {
                bool found = false;
                for (int k = 0; k < m; ++k) {
                    uint64_t v = L296006_small(k);
                    if (v == cand) { found = true; break; }
                    if (v != UINT64_MAX && v > cand) break;
                }
                if (!found) break;
                ++cand;
            }
            L296006_B[blen++] = (uint32_t)cand;
            next_cand = cand + 1;
        }
        uint64_t carry = 0;
        for (int j = 0; j < L296006_LIMBS; ++j) {
            uint64_t s = carry;
            for (int k = 0; k < m; ++k)
                s += (uint64_t)L296006_A[k][j] * L296006_B[m - 1 - k];
            L296006_A[m][j] = (uint32_t)s;
            carry = s >> 32;
        }
    }
    uint32_t num[L296006_LIMBS];
    for (int j = 0; j < L296006_LIMBS; ++j) num[j] = L296006_A[N][j];
    const uint32_t *den = L296006_A[N - 1];
    int q = 0;
    while (L296006_cmp(num, den) >= 0) { L296006_sub(num, den); ++q; }
    L296006_dig[0] = (unsigned char)q;
    for (int i = 1; i < L296006_DIGITS; ++i) {
        L296006_mul10(num);
        int d = 0;
        while (L296006_cmp(num, den) >= 0) { L296006_sub(num, den); ++d; }
        L296006_dig[i] = (unsigned char)d;
    }
}

seq_t a(idx_t n)
{
    L296006_init();
    if (n < 1 || n > L296006_DIGITS) return 0;
    return (seq_t)L296006_dig[(int)(n - 1)];
}
