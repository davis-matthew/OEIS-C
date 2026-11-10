/*
 * A317977  a(n) = A003010(n-2) mod (2^n - 1).
 * https://oeis.org/A317977
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003010(n-2) mod (2^n - 1), where A003010(0) = 4,
 *            A003010(k) = A003010(k-1)^2 - 2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 2970 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t A317977_a(idx_t n)
{
    size_t N = (size_t)n;
    size_t L = (N + 31) / 32;
    unsigned bs = (unsigned)(N % 32);
    size_t ws = N / 32;
    size_t PS = 2 * L + 4;
    uint32_t *x = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(PS, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *H = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    if (!x || !P || !S || !H) abort();
    uint32_t topmask = bs ? (((uint32_t)1 << bs) - 1) : 0xFFFFFFFFu;

    x[0] = (N == 2) ? 1u : 4u;

    for (size_t step = 0; step + 2 < N; step++) {
        size_t i, j;
        /* square */
        memset(P, 0, PS * sizeof(uint32_t));
        for (i = 0; i < L; i++) {
            uint64_t carry = 0;
            if (x[i] == 0) continue;
            for (j = 0; j < L; j++) {
                uint64_t cur = (uint64_t)P[i + j] + (uint64_t)x[i] * x[j] + carry;
                P[i + j] = (uint32_t)cur;
                carry = cur >> 32;
            }
            size_t k = i + L;
            while (carry) {
                uint64_t cur = (uint64_t)P[k] + carry;
                P[k] = (uint32_t)cur;
                carry = cur >> 32;
                k++;
            }
        }
        /* hi = P >> N */
        for (i = 0; i < L + 1; i++) {
            uint32_t w = P[i + ws] >> bs;
            if (bs) w |= P[i + ws + 1] << (32 - bs);
            H[i] = w;
        }
        /* S = lo + hi */
        {
            uint64_t carry = 0;
            for (i = 0; i < L + 1; i++) {
                uint64_t lo = 0;
                if (i < L) lo = P[i];
                if (i == L - 1) lo &= topmask;
                if (i >= L) lo = 0;
                uint64_t cur = lo + (uint64_t)H[i] + carry;
                S[i] = (uint32_t)cur;
                carry = cur >> 32;
            }
        }
        /* fold bit N if set (possibly more than once to be safe) */
        for (;;) {
            bool set;
            if (bs) set = (S[L - 1] >> bs) & 1u || S[L] != 0;
            else set = S[L] != 0;
            if (!set) break;
            /* clear bits >= N */
            if (bs) S[L - 1] &= topmask;
            S[L] = 0;
            uint64_t carry = 1;
            for (i = 0; i < L + 1 && carry; i++) {
                uint64_t cur = (uint64_t)S[i] + carry;
                S[i] = (uint32_t)cur;
                carry = cur >> 32;
            }
        }
        /* normalise M -> 0 */
        {
            bool allones = true;
            for (i = 0; i + 1 < L; i++) if (S[i] != 0xFFFFFFFFu) { allones = false; break; }
            if (allones && S[L - 1] == topmask) {
                for (i = 0; i < L + 1; i++) S[i] = 0;
            }
        }
        /* subtract 2 mod M */
        {
            uint32_t sub = 2;
            bool small = true;
            for (i = 1; i < L + 1; i++) if (S[i]) { small = false; break; }
            if (small && S[0] < 2) {
                sub = 2 - S[0];
                for (i = 0; i + 1 < L; i++) S[i] = 0xFFFFFFFFu;
                S[L - 1] = topmask;
                S[L] = 0;
            }
            uint64_t borrow = sub;
            for (i = 0; i < L + 1 && borrow; i++) {
                if ((uint64_t)S[i] >= borrow) { S[i] -= (uint32_t)borrow; borrow = 0; }
                else { S[i] = (uint32_t)((((uint64_t)1 << 32) + S[i]) - borrow); borrow = 1; }
            }
        }
        for (i = 0; i < L; i++) x[i] = S[i];
        x[L] = 0;
    }

    seq_t r = (seq_t)0;
    for (size_t b = N; b-- > 0; ) {
        unsigned bit = (x[b / 32] >> (b % 32)) & 1u;
        r = r + r + (seq_t)bit;
    }
    free(x); free(P); free(S); free(H);
    return r;
}

seq_t a(idx_t n)
{
    return A317977_a(n);
}
