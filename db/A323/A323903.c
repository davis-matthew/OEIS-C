/*
 * A323903  a(n) = A002487(A122111(n)).
 * https://oeis.org/A323903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(A122111(n)); A122111 = conjugate the partition of
 *            prime indices of n, A002487 = Stern's diatomic series, computed
 *            from the binary digits of the (big) number
 * Range:     u8 n <= 528, u16 n <= 3720, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 8490 (2026-11-08)
 * Helpers:   factor_u64, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/nth_prime.h"

static int a323903_cmp(const void *x, const void *y)
{
    idx_t vx = *(const idx_t *)x;
    idx_t vy = *(const idx_t *)y;
    return (vx > vy) - (vx < vy);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    idx_t indices[128];
    size_t cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t ix = (idx_t)prime_pi((idx_t)primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) indices[cnt++] = ix;
    }
    if (cnt == 0) return (seq_t)1;
    qsort(indices, cnt, sizeof(idx_t), a323903_cmp);

    /* big number N = A122111(n), little-endian 32-bit limbs */
    size_t cap = 16, len = 1;
    uint32_t *limb = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limb) abort();
    limb[0] = 1;

    size_t remaining = cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < cnt) {
        idx_t cur = indices[pos];
        size_t freq = 0;
        while (pos < cnt && indices[pos] == cur) { ++freq; ++pos; }
        idx_t length = cur - last_part + 1;
        uint64_t p = nth_prime((idx_t)remaining);
        for (idx_t t = 0; t < length; ++t) {
            uint64_t carry = 0;
            for (size_t i = 0; i < len; ++i) {
                uint64_t v = (uint64_t)limb[i] * p + carry;
                limb[i] = (uint32_t)v;
                carry = v >> 32;
            }
            if (carry) {
                if (len == cap) {
                    cap <<= 1;
                    limb = (uint32_t *)realloc(limb, cap * sizeof(uint32_t));
                    if (!limb) abort();
                }
                limb[len++] = (uint32_t)carry;
            }
        }
        remaining -= freq;
        last_part = cur + 1;
    }

    /* Stern: track (s(k), s(k+1)) over the bits of N from the top */
    int top = 31;
    while (top > 0 && !((limb[len - 1] >> top) & 1u)) --top;
    seq_t x = 1, y = 1;
    bool first = true;
    for (size_t li = len; li-- > 0; ) {
        int start = (li == len - 1) ? top : 31;
        for (int b = start; b >= 0; --b) {
            if (first) { first = false; continue; } /* leading 1 */
            if ((limb[li] >> b) & 1u) x = x + y;
            else y = x + y;
        }
    }
    free(limb);
    return x;
}
