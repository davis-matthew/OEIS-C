/*
 * A296224  Decimal expansion of the limiting ratio of terms in A296223.
 * https://oeis.org/A296224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   limit of A296223(n)/A296223(n-1), where A296223: a(0)=1, a(1)=3,
 *            b(0)=2, a(n) = Sum_{i=0..n-1} a(i)*b(n-1-i) - 1, b = complement
 *            of a. Digits come from native bignum long division of
 *            a(N)/a(N-1) for large N
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define A296224_L 40
#define A296224_N 600

static uint32_t A296224_A[A296224_N + 1][A296224_L];
static bool A296224_ready = false;

static int A296224_cmp(const uint32_t *x, const uint32_t *y)
{
    for (int i = A296224_L - 1; i >= 0; --i) {
        if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
    }
    return 0;
}

static void A296224_sub(uint32_t *x, const uint32_t *y)
{
    uint64_t borrow = 0;
    for (int i = 0; i < A296224_L; ++i) {
        uint64_t t = (uint64_t)x[i] - y[i] - borrow;
        x[i] = (uint32_t)t;
        borrow = (t >> 63) & 1;
    }
}

static void A296224_init(void)
{
    static unsigned short bseq[A296224_N + 2];
    static bool used[8192];
    memset(A296224_A, 0, sizeof(A296224_A));
    memset(used, 0, sizeof(used));
    A296224_A[0][0] = 1;
    A296224_A[1][0] = 3;
    used[1] = true; used[3] = true; used[2] = true;
    bseq[0] = 2;
    unsigned blen = 1;
    unsigned cand = 1;
    for (unsigned cur = 2; cur <= A296224_N; ++cur) {
        while (blen < cur) {
            while (used[cand]) cand++;
            bseq[blen++] = (unsigned short)cand;
            used[cand] = true;
        }
        uint32_t sum[A296224_L];
        memset(sum, 0, sizeof(sum));
        for (unsigned i = 0; i < cur; ++i) {
            uint64_t m = bseq[cur - 1 - i];
            uint64_t carry = 0;
            const uint32_t *ai = A296224_A[i];
            uint64_t c2 = 0;
            for (int k = 0; k < A296224_L; ++k) {
                uint64_t t = (uint64_t)ai[k] * m + carry;
                carry = t >> 32;
                uint64_t s = (uint64_t)sum[k] + (t & 0xFFFFFFFFu) + c2;
                sum[k] = (uint32_t)s;
                c2 = s >> 32;
            }
        }
        /* subtract 1 */
        for (int k = 0; k < A296224_L; ++k) {
            if (sum[k]) { sum[k]--; break; }
            sum[k] = 0xFFFFFFFFu;
        }
        memcpy(A296224_A[cur], sum, sizeof(sum));
        bool small = sum[0] < 8192;
        for (int k = 1; k < A296224_L && small; ++k) if (sum[k]) small = false;
        if (small) used[sum[0]] = true;
    }
    A296224_ready = true;
}

seq_t a(idx_t n)
{
    if (!A296224_ready) A296224_init();
    uint32_t rem[A296224_L];
    memcpy(rem, A296224_A[A296224_N], sizeof(rem));
    const uint32_t *den = A296224_A[A296224_N - 1];
    int d = 0;
    /* integer part */
    while (A296224_cmp(rem, den) >= 0) { A296224_sub(rem, den); d++; }
    if (n == 1) return (seq_t)d;
    for (idx_t step = 1; step < n; ++step) {
        uint64_t carry = 0;
        for (int k = 0; k < A296224_L; ++k) {
            uint64_t t = (uint64_t)rem[k] * 10 + carry;
            rem[k] = (uint32_t)t;
            carry = t >> 32;
        }
        d = 0;
        while (A296224_cmp(rem, den) >= 0) { A296224_sub(rem, den); d++; }
    }
    return (seq_t)d;
}
