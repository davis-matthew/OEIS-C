/*
 * A278379  Number of n X 2 0..1 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly three mistakes.
 * https://oeis.org/A278379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{15} Δ^k a(1) * C(n-1, k) (Newton forward form)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 22, u64 n <= 116, u128 n <= 210,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

/* ---- Big integer (base 1e9) ---- */
#define BIG_BASE 1000000000U
#define BIG_MAX 64   /* enough for 10000^15 (~10^60) */

typedef struct {
    size_t len;
    uint32_t d[BIG_MAX];
} BigInt;

static void big_init(BigInt *x, uint64_t v) {
    if (x == NULL) return;
    x->len = 1;
    x->d[0] = (uint32_t)(v % BIG_BASE);
    v /= BIG_BASE;
    if (v == 0) return;
    size_t i = 1;
    while (v && i < BIG_MAX) {
        x->d[i++] = (uint32_t)(v % BIG_BASE);
        v /= BIG_BASE;
    }
    x->len = i;
}

static void big_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static void big_norm(BigInt *x) {
    while (x->len > 1 && x->d[x->len - 1] == 0) x->len--;
}

static void big_add(BigInt *a, const BigInt *b) {
    uint64_t carry = 0;
    size_t max = (a->len > b->len) ? a->len : b->len;
    for (size_t i = 0; i < max; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        a->d[i] = (uint32_t)(sum % BIG_BASE);
        carry = sum / BIG_BASE;
    }
    if (carry) {
        a->d[max] = (uint32_t)carry;
        max++;
    }
    a->len = max;
}

static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

static void big_sub(BigInt *a, const BigInt *b) { /* a -= b, assumes a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) {
            diff += BIG_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    big_norm(a);
}

static void big_mul_uint(BigInt *x, uint64_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BIG_BASE);
        carry = prod / BIG_BASE;
    }
    while (carry && x->len < BIG_MAX) {
        x->d[x->len++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

static void big_div_uint(BigInt *x, uint32_t d) { /* divide by small d (d>0), assumes divisible */
    uint64_t rem = 0;
    for (int i = (int)x->len - 1; i >= 0; --i) {
        uint64_t cur = rem * BIG_BASE + x->d[i];
        x->d[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    big_norm(x);
}

/* Compute binomial coefficient C(n,k) into result */
static void big_binom(uint64_t n, uint64_t k, BigInt *result) {
    big_init(result, 1);
    for (uint64_t i = 1; i <= k; ++i) {
        big_mul_uint(result, n - k + i);
        big_div_uint(result, (uint32_t)i);
    }
}

/* Convert BigInt to decimal string (null-terminated) */
static void big_to_str(const BigInt *x, char *buf, size_t buflen) {
    size_t pos = 0;
    int i = (int)x->len - 1;
    pos += sprintf(buf + pos, "%u", x->d[i]);
    for (i = i - 1; i >= 0; --i) {
        pos += sprintf(buf + pos, "%09u", x->d[i]);
    }
    buf[pos] = '\0';
}

/* ---- Sequence implementation ---- */
seq_t a(idx_t n) {
    /* first 16 terms of the sequence */
    static const int64_t init[16] = {
        0,   /* a(1) */
        0,   /* a(2) */
        3,   /* a(3) */
        40,  /* a(4) */
        267, /* a(5) */
        1350,/* a(6) */
        5936,/* a(7) */
        23565,/* a(8) */
        84912,/* a(9) */
        278422,/* a(10) */
        835824,/* a(11) */
        2316601,/* a(12) */
        5980937,/* a(13) */
        14503972,/* a(14) */
        33282396,/* a(15) */
        72732358 /* a(16) */
    };

    /* precompute Newton coefficients Δ^k a(1) */
    static int64_t coef[16];
    static int coefs_computed = 0;
    if (!coefs_computed) {
        int64_t cur[16];
        memcpy(cur, init, sizeof(init));
        coef[0] = cur[0];
        for (int k = 1; k <= 15; ++k) {
            int sz = 16 - k;
            int64_t nxt[15];
            for (int i = 0; i < sz; ++i) {
                nxt[i] = cur[i + 1] - cur[i];
            }
            coef[k] = nxt[0];
            memcpy(cur, nxt, sz * sizeof(int64_t));
        }
        coefs_computed = 1;
    }

    if (n <= 16) {
        return (seq_t)init[(size_t)n - 1];
    }

    BigInt pos, neg;
    big_init(&pos, 0);
    big_init(&neg, 0);

    for (int k = 0; k <= 15; ++k) {
        int64_t c = coef[k];
        if (c == 0) continue;
        BigInt binom;
        big_binom(n - 1, k, &binom);
        BigInt term;
        big_copy(&term, &binom);
        uint64_t mul = (c > 0) ? (uint64_t)c : (uint64_t)(-c);
        big_mul_uint(&term, mul);
        if (c > 0) {
            big_add(&pos, &term);
        } else {
            big_add(&neg, &term);
        }
    }

    /* pos - neg  (assume pos >= neg) */
    BigInt res;
    big_copy(&res, &pos);
    big_sub(&res, &neg);

    /* convert to string and then to seq_t */
    char buf[200];
    big_to_str(&res, buf, sizeof(buf));
    return seq_from_str(buf);
}
