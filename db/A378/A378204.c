/*
 * A378204  Decimal expansion of the surface area of a triakis tetrahedron
 *          with unit shorter edge length.
 * https://oeis.org/A378204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of (5/3)*√11 (decimal expansion of the surface
 *            area of a triakis tetrahedron)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Simple arbitrary‑precision integer (base 1e9)                       */
/* ------------------------------------------------------------------ */
#define BASE 1000000000U

typedef struct {
    uint32_t *w; /* least significant word first */
    size_t len;
} BigInt;

/* create BigInt from uint64_t value */
static BigInt *bigint_from_uint64(uint64_t v)
{
    BigInt *a = (BigInt *)malloc(sizeof(BigInt));
    if (!a) return NULL;
    if (v == 0) {
        a->len = 1;
        a->w  = (uint32_t *)calloc(1, sizeof(uint32_t));
        return a;
    }
    size_t cnt = 0;
    uint64_t t = v;
    while (t) { t /= BASE; ++cnt; }
    a->len = cnt;
    a->w   = (uint32_t *)calloc(cnt, sizeof(uint32_t));
    t = v;
    for (size_t i = 0; i < cnt; ++i) {
        a->w[i] = (uint32_t)(t % BASE);
        t /= BASE;
    }
    return a;
}

/* free BigInt */
static void bigint_free(BigInt *a)
{
    if (a) {
        free(a->w);
        free(a);
    }
}

/* copy BigInt */
static BigInt *bigint_copy(const BigInt *a)
{
    BigInt *b = (BigInt *)malloc(sizeof(BigInt));
    if (!b) return NULL;
    b->len = a->len;
    b->w   = (uint32_t *)malloc(b->len * sizeof(uint32_t));
    memcpy(b->w, a->w, b->len * sizeof(uint32_t));
    return b;
}

/* multiply by small integer (in place) */
static void bigint_mul_small(BigInt *a, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->w[i] * m + carry;
        a->w[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    if (carry) {
        a->w = (uint32_t *)realloc(a->w, (a->len + 1) * sizeof(uint32_t));
        a->w[a->len] = (uint32_t)carry;
        ++a->len;
    }
}

/* add small integer (in place) */
static void bigint_add_small(BigInt *a, uint32_t add)
{
    uint64_t sum = (uint64_t)a->w[0] + add;
    a->w[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        uint64_t s = (uint64_t)a->w[i] + carry;
        a->w[i] = (uint32_t)(s % BASE);
        carry   = s / BASE;
        ++i;
    }
    if (carry) {
        a->w = (uint32_t *)realloc(a->w, (a->len + 1) * sizeof(uint32_t));
        a->w[a->len] = (uint32_t)carry;
        ++a->len;
    }
}

/* add small integer to a copy */
static BigInt *bigint_add_small_copy(const BigInt *a, uint32_t add)
{
    BigInt *b = bigint_copy(a);
    bigint_add_small(b, add);
    return b;
}

/* multiply copy by small integer */
static BigInt *bigint_mul_small_copy(const BigInt *a, uint32_t m)
{
    BigInt *b = bigint_copy(a);
    bigint_mul_small(b, m);
    return b;
}

/* compare a and b: return -1 if a<b, 0 if a==b, 1 if a>b */
static int bigint_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i > 0; --i) {
        uint32_t x = a->w[i - 1];
        uint32_t y = b->w[i - 1];
        if (x != y) return (x < y) ? -1 : 1;
    }
    return 0;
}

/* subtract b from a (a >= b), in place */
static void bigint_sub_eq(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->w[i] - (i < b->len ? b->w[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->w[i] = (uint32_t)sub;
    }
    /* trim leading zeros */
    while (a->len > 1 && a->w[a->len - 1] == 0) --a->len;
}

/* ------------------------------------------------------------------ */
/*  Data needed by the sequence function                              */
/* ------------------------------------------------------------------ */
static bool initialized = false;
static uint8_t *output = NULL;          /* cache of computed digits */
static size_t out_cap   = 0;
static size_t out_len   = 0;

static BigInt *sqrtRoot = NULL;         /* current root during extraction */
static BigInt *sqrtRem  = NULL;         /* current remainder during extraction */
static uint8_t div_rem = 0;             /* remainder for division by 3 */

/* ------------------------------------------------------------------ */
/*  Produce the next decimal digit of sqrt(275)                       */
/* ------------------------------------------------------------------ */
static int next_sqrt_digit(void)
{
    /* bring down two zeroes */
    bigint_mul_small(sqrtRem, 100);

    /* temp = sqrtRoot * 20 */
    BigInt *temp = bigint_mul_small_copy(sqrtRoot, 20);

    /* find the largest decimal digit */
    BigInt *bestCand = NULL;
    int best_d = 0;
    for (int d = 9; d >= 0; --d) {
        BigInt *cand = bigint_add_small_copy(temp, (uint32_t)d);
        cand = bigint_mul_small_copy(cand, (uint32_t)d);
        if (bigint_cmp(cand, sqrtRem) <= 0) {  /* candidate fits */
            best_d = d;
            bestCand = cand;
            break;
        }
        bigint_free(cand);
    }

    /* subtract the chosen candidate from the remainder */
    bigint_sub_eq(sqrtRem, bestCand);

    /* update the root: root = root*10 + d */
    bigint_mul_small(sqrtRoot, 10);
    bigint_add_small(sqrtRoot, (uint32_t)best_d);

    /* free temporaries */
    bigint_free(temp);
    bigint_free(bestCand);

    return best_d;
}

/* ------------------------------------------------------------------ */
/*  Main function of the sequence                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        out_cap = 64;
        output = (uint8_t *)malloc(out_cap);

        /* Compute integer part of sqrt(275) = 5*sqrt(11) */
        BigInt *rootTmp = bigint_from_uint64(0);
        BigInt *remTmp  = bigint_from_uint64(0);
        unsigned rootInt = 0;
        unsigned groups[2] = {2, 75};
        for (size_t gi = 0; gi < 2; ++gi) {
            /* bring down the next group (two digits) */
            bigint_mul_small(remTmp, 100);
            bigint_add_small(remTmp, groups[gi]);

            /* temp = rootTmp * 20 */
            BigInt *temp = bigint_mul_small_copy(rootTmp, 20);

            /* find the next digit */
            BigInt *cand = NULL;
            int best = 0;
            for (int d = 9; d >= 0; --d) {
                cand = bigint_add_small_copy(temp, (uint32_t)d);
                cand = bigint_mul_small_copy(cand, (uint32_t)d);
                if (bigint_cmp(cand, remTmp) <= 0) {
                    best = d;
                    break;
                }
                bigint_free(cand);
                cand = NULL;
            }

            /* subtract the chosen candidate */
            if (cand == NULL) {
                cand = bigint_add_small_copy(temp, (uint32_t)best);
                cand = bigint_mul_small_copy(cand, (uint32_t)best);
            }
            bigint_sub_eq(remTmp, cand);

            /* update root */
            bigint_mul_small(rootTmp, 10);
            bigint_add_small(rootTmp, (uint32_t)best);

            /* remember the integer root as a normal integer */
            rootInt = rootInt * 10 + (unsigned)best;

            bigint_free(temp);
            bigint_free(cand);
        }

        sqrtRoot = rootTmp;
        sqrtRem  = remTmp;

        /* Divide the integer part by 3 to produce the first quot. digit(s) */
        unsigned qnum = rootInt / 3;
        div_rem = rootInt % 3;

        if (qnum == 0) {
            output[out_len++] = 0;
        } else {
            uint8_t tmp[20];
            size_t tlen = 0;
            unsigned v = qnum;
            while (v) { tmp[tlen++] = (uint8_t)(v % 10); v /= 10; }
            for (size_t i = 0; i < tlen; ++i) output[out_len++] = tmp[tlen - 1 - i];
        }
    }

    while (out_len < n) {
        int d = next_sqrt_digit();          /* next digit of sqrt(275) */
        int q = (div_rem * 10 + d) / 3;     /* next quotient digit */
        div_rem = (div_rem * 10 + d) % 3;   /* update remainder */

        if (out_len == out_cap) {
            out_cap *= 2;
            output = (uint8_t *)realloc(output, out_cap);
        }
        output[out_len++] = (uint8_t)q;
    }

    return (seq_t)output[n - 1];
}
