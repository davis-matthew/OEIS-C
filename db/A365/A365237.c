/*
 * A365237  Decimal expansion of 1/A033307 (decimal Champernowne constant).
 * https://oeis.org/A365237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   champernowne_first_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the decimal expansion of 1 / 0.123456789101112… (Champernowne constant)
 * The function performs arbitrary‑precision integer arithmetic to generate the
 * required digit.  It multiplies the remainder of the long division by 10 and
 * finds the largest k∈[0,9] such that k⋅N ≤ remainder, where N is the
 * Champernowne prefix used as the denominator.  The prefix length is
 * m = n + 10 to guarantee that the first n digits are correct. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../utils/champernowne_first_digits.h"

/* Big integer base. 1e9 fits into 32 bits. */
#define BIGBASE 1000000000U

typedef struct {
    uint32_t *d;   /* little‑endian limbs 0 … d[len-1] */
    size_t len;    /* number of limbs, len ≥ 1 */
} Big;

static void big_init(Big *a) {
    a->d = NULL;
    a->len = 0;
}

static void big_free(Big *a) {
    free(a->d);
    a->d = NULL;
    a->len = 0;
}

static void big_copy(Big *dst, const Big *src) {
    dst->len = src->len;
    dst->d = (uint32_t*)malloc(dst->len * sizeof(uint32_t));
    memcpy(dst->d, src->d, dst->len * sizeof(uint32_t));
}

static int big_cmp(const Big *a, const Big *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

static void big_sub(Big *a, const Big *b) {  /* a ≥ b */
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i];
        if (i < b->len) sub -= b->d[i];
        sub -= borrow;
        if ((int64_t)sub < 0) {
            sub += BIGBASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    /* remove leading zero limbs */
    while (a->len > 1 && a->d[a->len-1] == 0) a->len--;
}

static void big_mul_uint(Big *a, uint32_t m) {
    if (m == 0) {
        free(a->d);
        a->len = 1;
        a->d = (uint32_t*)malloc(sizeof(uint32_t));
        a->d[0] = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = prod % BIGBASE;
        carry = prod / BIGBASE;
    }
    if (carry) {
        a->d = (uint32_t*)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
        a->d[a->len] = carry;
        a->len++;
    }
}

static void big_mul_uint_const(const Big *a, uint32_t m, Big *res) {
    if (m == 0) {
        res->len = 1;
        res->d = (uint32_t*)malloc(sizeof(uint32_t));
        res->d[0] = 0;
        return;
    }
    res->len = a->len;
    res->d = (uint32_t*)malloc(res->len * sizeof(uint32_t));
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        res->d[i] = prod % BIGBASE;
        carry = prod / BIGBASE;
    }
    if (carry) {
        res->d = (uint32_t*)realloc(res->d, (res->len + 1) * sizeof(uint32_t));
        res->d[res->len] = carry;
        res->len++;
    }
}

static void big_from_string(Big *a, const char *s) {
    size_t slen = strlen(s);
    if (slen == 0) {
        a->len = 1;
        a->d = (uint32_t*)malloc(sizeof(uint32_t));
        a->d[0] = 0;
        return;
    }
    size_t groups = (slen + 8) / 9;          /* each group holds 9 digits */
    a->len = groups;
    a->d = (uint32_t*)calloc(groups, sizeof(uint32_t));
    size_t pos = slen;
    for (size_t g = 0; g < groups; ++g) {
        uint32_t val = 0;
        uint32_t mul = 1;
        for (int k = 0; k < 9 && pos > 0; ++k) {
            --pos;
            val += (uint32_t)(s[pos] - '0') * mul;
            mul *= 10;
        }
        a->d[g] = val;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n<1 */

    /* use a safety margin of 10 extra digits */
    idx_t m = n + 10;

    /* build Champernowne prefix N of length m */
    char *cham = (char*)malloc(m + 1);
    champernowne_first_digits(m, cham);
    Big N;
    big_from_string(&N, cham);
    free(cham);

    /* build the numerator 10^m as string and parse */
    char *powbuf = (char*)malloc(m + 2);
    powbuf[0] = '1';
    for (idx_t i = 1; i <= m; ++i) powbuf[i] = '0';
    powbuf[m + 1] = '\0';

    Big numer;
    big_from_string(&numer, powbuf);
    free(powbuf);

    /* precompute N*d for d=0..9 */
    Big prod[10];
    for (int d = 0; d <= 9; ++d) {
        big_mul_uint_const(&N, (uint32_t)d, &prod[d]);
    }

    /* integer part (a(1)) */
    int intpart = 0;
    for (int d = 9; d >= 0; --d) {
        if (big_cmp(&numer, &prod[d]) >= 0) {
            intpart = d;
            big_sub(&numer, &prod[d]);   /* remainder now in numer */
            break;
        }
    }

    if (n == 1) {
        /* free all allocations */
        for (int d = 0; d <= 9; ++d) big_free(&prod[d]);
        big_free(&N);
        big_free(&numer);
        return (seq_t)intpart;
    }

    /* copy remainder for decimal digits */
    Big r;
    big_copy(&r, &numer);

    seq_t result = 0;
    for (idx_t idx = 2; idx <= n; ++idx) {
        big_mul_uint(&r, 10);
        int d;
        for (d = 9; d >= 0; --d) {
            if (big_cmp(&r, &prod[d]) >= 0) break;
        }
        big_sub(&r, &prod[d]);
        result = d;
        if (idx == n) break;
    }

    /* cleanup */
    for (int d = 0; d <= 9; ++d) big_free(&prod[d]);
    big_free(&N);
    big_free(&numer);
    big_free(&r);

    return (seq_t)result;
}
