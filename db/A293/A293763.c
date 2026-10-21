/*
 * A293763  Numbers k such that (d(k), d(k+1)) = (1,0) in the base-2 digits
 *          d(i) of sqrt(2).
 * https://oeis.org/A293763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth position k such that the binary digits * of
 *            sqrt(2), d(k)=1 and d(k+1)=0. The function computes this * by
 *            generating the binary expansion of sqrt(2) digit by digit *
 *            using only integer arithmetic. It uses a small big‑integer *
 *            implementation sufficient to generate up to about 100...
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ---------- small big‑integer implementation ---------- */
#define BIGCAP 1600 /* enough for ~100k bits (1600*64) */

typedef struct {
    uint64_t *d;   /* little‑endian words, d[0] lowest word */
    size_t len;    /* number of used words (non‑zero) */
    size_t cap;    /* capacity in words */
} BigInt;

static void big_init(BigInt *x, size_t cap)
{
    x->d = (uint64_t *)malloc(cap * sizeof(uint64_t));
    x->len = 0;
    x->cap = cap;
}

static void big_free(BigInt *x)
{
    free(x->d);
    x->d = NULL;
    x->len = 0;
    x->cap = 0;
}

static void big_trim(BigInt *x)
{
    while (x->len > 0 && x->d[x->len - 1] == 0)
        x->len--;
}

static void big_set_uint(BigInt *x, uint64_t v)
{
    x->len = (v == 0) ? 0 : 1;
    if (v != 0)
        x->d[0] = v;
}

static void big_copy(BigInt *dest, const BigInt *src)
{
    dest->len = src->len;
    memcpy(dest->d, src->d, src->len * sizeof(uint64_t));
}

static int big_compare(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; )
        if (a->d[i] != b->d[i])
            return (a->d[i] > b->d[i]) ? 1 : -1;
    return 0;
}

static void big_shift_left(BigInt *x, int bits) /* bits = 1 or 2 */
{
    if (bits == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t new_carry = x->d[i] >> (64 - bits);
        x->d[i] = (x->d[i] << bits) | carry;
        carry = new_carry;
    }
    if (carry != 0) {
        /* need one more word */
        if (x->len == x->cap) {
            /* grow capacity if needed */
            x->cap *= 2;
            x->d = (uint64_t *)realloc(x->d, x->cap * sizeof(uint64_t));
        }
        x->d[x->len++] = carry;
    }
}

static void big_subtract(BigInt *a, const BigInt *b) /* assumes a >= b */
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t ai = a->d[i];
        uint64_t bi = (i < b->len) ? b->d[i] : 0;
        uint64_t sub = bi + borrow;
        if (ai >= sub) {
            a->d[i] = ai - sub;
            borrow = 0;
        } else {
            a->d[i] = ai + (uint64_t)(-1) - sub + 1;
            borrow = 1;
        }
    }
    /* borrow should be zero if a>=b */
    big_trim(a);
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    BigInt p, r, cand;
    big_init(&p, BIGCAP);
    big_init(&r, BIGCAP);
    big_init(&cand, BIGCAP);

    big_set_uint(&p, 1);   /* integer part of sqrt(2) */
    big_set_uint(&r, 1);   /* remainder: 2 - 1^2 */

    uint64_t d_prev = 1;   /* d(1) */
    uint64_t d_cur;
    uint64_t pos = 1;      /* position of d_prev */
    uint64_t found = 0;
    seq_t answer = 0;

    while (found < n) {
        /* generate next digit d(pos+1) */
        pos++;
        big_shift_left(&r, 2);   /* r <<= 2 */

        /* candidate = (p << 2) | 1 */
        big_copy(&cand, &p);
        big_shift_left(&cand, 2);
        cand.d[0] |= 1ULL;

        if (big_compare(&r, &cand) >= 0) {
            /* take the digit 1 */
            big_subtract(&r, &cand);
            d_cur = 1;
            big_shift_left(&p, 1);
            p.d[0] |= 1ULL;
        } else {
            /* take the digit 0 */
            d_cur = 0;
            big_shift_left(&p, 1);
        }

        if (d_prev == 1 && d_cur == 0) {
            found++;
            if (found == n)
                answer = pos - 1;  /* k = position of 1 in the pair */
        }
        d_prev = d_cur;
    }

    /* clean up */
    big_free(&p);
    big_free(&r);
    big_free(&cand);

    return (seq_t)answer;
}
