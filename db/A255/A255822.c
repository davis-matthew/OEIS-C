/*
 * A255822  Number of nonzero terms in A117943 up to index 2^n: a(n) =
 *          Sum_{k=1..2^n} A117943(k).
 * https://oeis.org/A255822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(2^n), where S(N) = Sum_{k=1..N} A117943(k) satisfies *
 *            S(N) = S(floor(N/3)) + S(N - floor(N/3)), S(0) = S(1) = 0, S(2)
 *            = 1. * Each recursion node is floor(2^(n+i)/3^(i+j)) + o, with i
 *            ceil(2v/3)-steps, * j floor(v/3)-steps and a small offset o in
 *            -1..4. Base-3 digits of 2^m give * each n...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 1949 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#define A255822_K 12
#define A255822_SMALL 531441u   /* 3^12 */
#define A255822_NOFF 6          /* offsets -1..4 */

static uint32_t *A255822_tab = NULL;   /* S(v), 0 <= v < SMALL + 8 */

static void A255822_init(void)
{
    if (A255822_tab) return;
    size_t sz = (size_t)A255822_SMALL + 8;
    unsigned char *s = mem_new(unsigned char, sz);
    uint32_t *t = mem_new(uint32_t, sz);
    s[0] = 0; s[1] = 0; s[2] = 1;
    t[0] = 0; t[1] = 0; t[2] = 1;
    for (size_t k = 3; k < sz; k++) {
        s[k] = (k % 3 == 0) ? s[k / 3] : s[k - k / 3];
        t[k] = t[k - 1] + s[k];
    }
    mem_free(s);
    A255822_tab = t;
}

/* S(B + j) where B = floor(D / 3^t) is known to be < 3^K */
static uint64_t A255822_leaf(const unsigned char *d, size_t len, size_t t, long long j)
{
    long long B = 0;
    if (t < len) {
        for (size_t i = len; i-- > t;) B = B * 3 + d[i];
    }
    long long v = B + j;
    if (v < 0) return 0;
    return A255822_tab[v];
}

static void A255822_copy(uint64_t *out, size_t w, const uint64_t *src, size_t ws)
{
    size_t i = 0;
    if (ws > w) ws = w;
    for (; i < ws; i++) out[i] = src[i];
    for (; i < w; i++) out[i] = 0;
}

static void A255822_setsmall(uint64_t *out, size_t w, uint64_t x)
{
    out[0] = x;
    for (size_t i = 1; i < w; i++) out[i] = 0;
}

static void A255822_add(uint64_t *out, size_t w, const uint64_t *src, size_t ws)
{
    uint64_t carry = 0;
    size_t i = 0;
    if (ws > w) ws = w;
    for (; i < ws; i++) {
        uint64_t s = out[i] + carry;
        uint64_t c1 = (s < carry);
        uint64_t s2 = s + src[i];
        uint64_t c2 = (s2 < s);
        out[i] = s2;
        carry = c1 + c2;
    }
    for (; carry && i < w; i++) {
        out[i] += 1;
        carry = (out[i] == 0);
    }
}

static void A255822_addsmall(uint64_t *out, size_t w, uint64_t x)
{
    uint64_t s = out[0] + x;
    uint64_t carry = (s < x);
    out[0] = s;
    for (size_t i = 1; carry && i < w; i++) {
        out[i] += 1;
        carry = (out[i] == 0);
    }
}

seq_t a(idx_t n)
{
    A255822_init();

    size_t N = (size_t)n;
    size_t Atop = 2 * N + 10;
    size_t m = N + Atop + 1;
    size_t cap = (m * 2) / 3 + 8;
    size_t WL = (N + 2) / 64 + 2;
    size_t rowCap = N + 2;

    unsigned char *D0 = mem_new(unsigned char, cap);
    unsigned char *D1 = mem_new(unsigned char, cap);
    size_t len0 = 0, len1 = 1;

    /* D1 = 2^m in base 3 */
    D1[0] = 1;
    for (size_t s = 0; s < m; s++) {
        unsigned carry = 0;
        for (size_t i = 0; i < len1; i++) {
            unsigned x = 2u * D1[i] + carry;
            D1[i] = (unsigned char)(x % 3);
            carry = x / 3;
        }
        if (carry) D1[len1++] = (unsigned char)carry;
    }

    uint64_t *prev = mem_new(uint64_t, rowCap * A255822_NOFF * WL);
    uint64_t *cur = mem_new(uint64_t, rowCap * A255822_NOFF * WL);
    size_t *wprev = mem_new(size_t, rowCap);
    size_t *wcur = mem_new(size_t, rowCap);
    size_t bcPrev = 0;

    uint64_t *ansLimbs = NULL;
    size_t ansW = 0;
    uint64_t ansSmall = 0;
    bool ansIsSmall = true;

    for (size_t a_ = Atop;; a_--) {
        /* D1 = 2^(n+a+1); D0 = 2^(n+a) */
        for (size_t i = 0; i < len1; i++) D0[i] = D1[i];
        len0 = len1;
        {
            unsigned rem = 0;
            for (size_t i = len0; i-- > 0;) {
                unsigned x = rem * 3 + D0[i];
                D0[i] = (unsigned char)(x / 2);
                rem = x & 1u;
            }
            while (len0 > 0 && D0[len0 - 1] == 0) len0--;
        }

        size_t bc = (len0 > a_ + A255822_K) ? len0 - a_ - A255822_K : 0;
        if (bc > rowCap) bc = rowCap;

        for (size_t b = bc; b-- > 0;) {
            size_t t = a_ + b;
            int r = D0[t];
            int d1 = (t < len1) ? D1[t] : 0;
            int e = (d1 + 6 - 2 * r) % 3;
            int delta = (2 * r + e) / 3;
            size_t w = (((len0 - t) * 1585) / 1000 + 4) / 64 + 1;
            if (w > WL) w = WL;
            uint64_t *dest = cur + (b * A255822_NOFF) * WL;

            for (int jj = 0; jj < A255822_NOFF; jj++) {
                int j = jj - 1;
                int q = (r + j + 3) / 3 - 1;      /* floor((r+j)/3) */
                int jf = q;
                int jg = r + j - q - delta;
                uint64_t *out = dest + (size_t)jj * WL;

                if (b + 1 < bc) {
                    A255822_copy(out, w,
                                 cur + ((b + 1) * A255822_NOFF + (size_t)(jf + 1)) * WL,
                                 wcur[b + 1]);
                } else {
                    A255822_setsmall(out, w, A255822_leaf(D0, len0, t + 1, jf));
                }
                if (b < bcPrev) {
                    A255822_add(out, w,
                                prev + (b * A255822_NOFF + (size_t)(jg + 1)) * WL,
                                wprev[b]);
                } else {
                    A255822_addsmall(out, w, A255822_leaf(D1, len1, t + 1, jg));
                }
            }
            wcur[b] = w;
        }

        if (a_ == 0) {
            if (bc > 0) {
                ansIsSmall = false;
                ansW = wcur[0];
                ansLimbs = cur + (0 * A255822_NOFF + 1) * WL;
            } else {
                ansSmall = A255822_leaf(D0, len0, 0, 0);
            }
            break;
        }

        {
            uint64_t *tp = prev; prev = cur; cur = tp;
            size_t *tw = wprev; wprev = wcur; wcur = tw;
            unsigned char *td = D1; D1 = D0; D0 = td;
            len1 = len0;
            bcPrev = bc;
        }
    }

    seq_t ans = 0;
    if (ansIsSmall) {
        for (int sh = 56; sh >= 0; sh -= 8)
            ans = (seq_t)((ans << 8) + (seq_t)((ansSmall >> sh) & 255u));
    } else {
        size_t top = ansW;
        while (top > 0 && ansLimbs[top - 1] == 0) top--;
        for (size_t i = top; i-- > 0;) {
            uint64_t x = ansLimbs[i];
            for (int sh = 56; sh >= 0; sh -= 8)
                ans = (seq_t)((ans << 8) + (seq_t)((x >> sh) & 255u));
        }
    }

    mem_free(D0);
    mem_free(D1);
    mem_free(prev);
    mem_free(cur);
    mem_free(wprev);
    mem_free(wcur);
    return ans;
}
