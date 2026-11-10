/*
 * A331601  a(n) = A002487(A241909(n)).
 * https://oeis.org/A331601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(A241909(n)); A241909(n) is built exactly as a
 *            multi-limb integer (it can far exceed 64 bits), then Stern's
 *            diatomic A002487 is evaluated over its binary expansion:
 *            x=1,y=0; for each bit from LSB: bit ? y+=x : x+=y; result y
 * Range:     u8 n <= 45, u16 n <= 121, u32 n <= 297, u64 n <= 673,
 *            u128 n <= 1621, big any n
 * Validated: standard, pass, 109 terms through n = 8740 (2026-11-08)
 * Helpers:   factor_u64, nth_prime, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi_u64.h"
#include "../utils/nth_prime.h"

typedef struct {
    uint32_t *d;
    size_t len;
    size_t cap;
} A331601_big;

static void A331601_big_init(A331601_big *b) {
    b->cap = 16;
    b->d = (uint32_t *)malloc(b->cap * sizeof(uint32_t));
    if (!b->d) abort();
    b->d[0] = 1;
    b->len = 1;
}

static void A331601_big_mul_small(A331601_big *b, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t t = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        if (b->len == b->cap) {
            b->cap *= 2;
            b->d = (uint32_t *)realloc(b->d, b->cap * sizeof(uint32_t));
            if (!b->d) abort();
        }
        b->d[b->len++] = (uint32_t)carry;
    }
}

/* Stern's diatomic series evaluated on a multi-limb number. */
static seq_t A331601_stern_big(const A331601_big *b) {
    seq_t x = 1, y = 0;
    size_t len = b->len;
    while (len > 0 && b->d[len - 1] == 0) --len;
    for (size_t i = 0; i < len; ++i) {
        uint32_t w = b->d[i];
        int nbits = 32;
        if (i == len - 1) {
            nbits = 0;
            uint32_t t = w;
            while (t) { ++nbits; t >>= 1; }
        }
        for (int k = 0; k < nbits; ++k) {
            if ((w >> k) & 1u) y += x;
            else x += y;
        }
    }
    return y;
}

seq_t a(idx_t n) {
    if (n <= (idx_t)1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    uint64_t indices[72];
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pi = prime_pi_u64(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) indices[total++] = pi;
    }

    A331601_big b;
    A331601_big_init(&b);

    if (total == 1) {
        /* a(p_i) = 2^i */
        for (uint64_t e = 0; e < indices[0]; ++e) A331601_big_mul_small(&b, 2u);
    } else {
        for (size_t j = 0; j < total; ++j) {
            uint32_t cp = (uint32_t)nth_prime((idx_t)(j + 1));
            uint64_t e;
            if (j == 0) e = indices[0] - 1;
            else if (j == total - 1) e = 1 + indices[j] - indices[j - 1];
            else e = indices[j] - indices[j - 1];
            for (uint64_t k = 0; k < e; ++k) A331601_big_mul_small(&b, cp);
        }
    }

    seq_t r = A331601_stern_big(&b);
    free(b.d);
    return r;
}
