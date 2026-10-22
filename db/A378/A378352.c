/*
 * A378352  Decimal expansion of the volume of a (small) triakis octahedron
 *          with unit shorter edge length.
 * https://oeis.org/A378352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit (1‑based) of sqrt(2)+3/2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* -------- arbitrary‑precision helpers for the internal calculation -------- */
#define BASE 1000000000u            /* 10^9  < 2^32, fits in uint32_t */
#define MAX_LIMBS 2000              /* enough for 10 000 decimal digits */

typedef struct {
    size_t len;                     /* number of limbs used (>=1)       */
    uint32_t d[MAX_LIMBS];          /* limbs, little‑endian             */
} BigInt;

static void init_bigint(BigInt *a, uint32_t v)
{
    a->len = 1;
    a->d[0] = v;
}

static void normalize(BigInt *a)
{
    while (a->len > 1 && a->d[a->len - 1] == 0)
        --a->len;
}

static void mul_small_assign(BigInt *a, uint32_t m)
{
    if (m == 0) {
        a->len = 1;
        a->d[0] = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void mul_small_copy(const BigInt *src, uint32_t m, BigInt *dst)
{
    if (m == 0) {
        dst->len = 1;
        dst->d[0] = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < src->len; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * m + carry;
        dst->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    dst->len = src->len;
    while (carry) {
        dst->d[dst->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void add_small_assign(BigInt *a, uint32_t aadd)
{
    if (aadd == 0) return;
    uint64_t carry = aadd;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void copy_bigint(const BigInt *src, BigInt *dst)
{
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static int compare_bigint(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

static void sub_assign(BigInt *a, const BigInt *b) /* assumes a >= b */
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - borrow -
                       (i < b->len ? b->d[i] : 0);
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    normalize(a);
}

/* -------- context for incremental sqrt(2) digit extraction -------- */
typedef struct {
    BigInt root;          /* current root (without the decimal point) */
    BigInt rem;           /* remainder in the extraction process      */
    uint8_t *digits;      /* current list of fractional digits (0‑based) */
    size_t digits_cap;    /* allocated capacity of the digits array    */
    size_t digits_len;    /* number of digits computed so far          */
} sqrt2_ctx;

static sqrt2_ctx ctx = {.root = {0}, .rem = {0},
                        .digits = NULL, .digits_cap = 0,
                        .digits_len = 0};

/* initialisation of the context */
static void init_ctx(void)
{
    init_bigint(&ctx.root, 1);      /* sqrt(2) integer part = 1 */
    init_bigint(&ctx.rem, 1);       /* rem = 2 - 1^2 = 1       */
    ctx.digits = NULL;
    ctx.digits_cap = 0;
    ctx.digits_len = 0;
}

/* generate digits until we have at least 'needed' digits */
static void ensure_digits_up_to(size_t needed)
{
    if (ctx.root.len == 0) init_ctx();  /* first call */

    /* temporary BigInt objects reused each iteration */
    static BigInt base, temp, prod;

    while (ctx.digits_len < needed) {
        /* step: multiply remainder by 100 to bring down two zeros */
        mul_small_assign(&ctx.rem, 100);

        /* base = root * 20 */
        mul_small_copy(&ctx.root, 20, &base);

        /* find the largest y (0..9) with (base + y) * y <= rem */
        uint32_t y;
        for (y = 9; ; --y) {
            copy_bigint(&base, &temp);
            add_small_assign(&temp, y);
            mul_small_copy(&temp, y, &prod);
            if (compare_bigint(&prod, &ctx.rem) <= 0) {
                sub_assign(&ctx.rem, &prod);
                break;
            }
            if (y == 0) break; /* safety, though y=0 always fits */
        }

        /* update root: root = root * 10 + y */
        mul_small_assign(&ctx.root, 10);
        add_small_assign(&ctx.root, y);

        /* store the digit */
        if (ctx.digits_len >= ctx.digits_cap) {
            size_t newcap = ctx.digits_cap ? ctx.digits_cap * 2 : 128;
            uint8_t *newbuf = (uint8_t *)realloc(ctx.digits, newcap);
            ctx.digits = newbuf;
            ctx.digits_cap = newcap;
        }
        ctx.digits[ctx.digits_len++] = (uint8_t)y;
    }
}

/* -------- the required sequence function -------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return (seq_t)2;      /* integer part of sqrt(2)+3/2 */

    size_t need = (size_t)(n - 1);    /* number of fractional digits required */
    ensure_digits_up_to(need);

    if (n == 2) {                     /* first fractional digit + 5 */
        uint8_t d = ctx.digits[0] + 5;
        return (seq_t)d;
    }
    /* subsequent digits are unchanged from sqrt(2) */
    uint8_t d = ctx.digits[n - 2];
    return (seq_t)d;
}
