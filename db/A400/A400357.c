/*
 * A400357  Decimal expansion of (sqrt(33) - 3)/8.
 * https://oeis.org/A400357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy, new
 * Formula:   a(n) = nth decimal digit of (sqrt(33)-3)/8 (0‑based)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000U

typedef struct {
    size_t len;      /* number of used limbs (least significant first) */
    uint32_t *d;     /* pointer to limbs */
} BigInt;

/* Allocate a BigInt with given capacity (number of limbs) */
static BigInt bi_alloc(size_t cap)
{
    BigInt a;
    a.len = 0;
    a.d   = (uint32_t *)calloc(cap, sizeof(uint32_t));
    return a;
}

/* Free a BigInt */
static void bi_free(BigInt *a)
{
    free(a->d);
    a->len = 0;
    a->d   = NULL;
}

/* set to unsigned integer value (fits in uint64_t) */
static void bi_set_uint(BigInt *a, uint64_t v)
{
    a->len = 0;
    while (v > 0) {
        a->d[a->len++] = (uint32_t)(v % BASE);
        v /= BASE;
    }
    if (a->len == 0) a->len = 1, a->d[0] = 0;
}

/* copy src into dst */
static void bi_copy(BigInt *dst, const BigInt *src)
{
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* multiply by small integer m (m <= 1000) */
static void bi_mul_small(BigInt *a, uint32_t m)
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
        carry   = prod / BASE;
    }
    while (carry > 0) {
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* add small integer m (m < BASE) */
static void bi_add_small(BigInt *a, uint32_t m)
{
    uint64_t sum = (uint64_t)a->d[0] + m;
    a->d[0]      = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry > 0 && i < a->len) {
        sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
        ++i;
    }
    if (carry > 0) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* comparison: return 1 if a > b, -1 if a < b, 0 if equal */
static int bi_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

/* subtraction: a = a - b, assumes a >= b */
static void bi_sub(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - borrow;
        if (i < b->len) diff -= b->d[i];
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    /* trim leading zeros */
    while (a->len > 1 && a->d[a->len - 1] == 0) {
        --a->len;
    }
}

/* division by small integer d (d <= 1000), a becomes quotient; return remainder */
static uint32_t bi_div_small(BigInt *a, uint32_t d)
{
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        uint64_t cur = rem * BASE + a->d[i];
        a->d[i] = (uint32_t)(cur / d);
        rem     = cur % d;
    }
    /* trim leading zeros */
    while (a->len > 1 && a->d[a->len - 1] == 0) {
        --a->len;
    }
    return (uint32_t)rem;
}

/* modulo of BigInt by small integer mod (mod <= 1e9) */
static uint32_t bi_mod_small(const BigInt *a, uint32_t mod)
{
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        rem = (rem * BASE + a->d[i]) % mod;
    }
    return (uint32_t)rem;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* capacity for big integers: about (n+1) decimal digits -> (n+9)/9 limbs, plus safety */
    size_t cap = (n / 9) + 10;

    BigInt p = bi_alloc(cap);   /* sqrt(33) * 10^k so far */
    BigInt r = bi_alloc(cap);   /* remainder used by digit extraction */
    BigInt q = bi_alloc(cap);   /* temporary: p * 20 */
    BigInt t = bi_alloc(cap);   /* temporary for (q+m)*m */
    BigInt d = bi_alloc(cap);   /* 3 * 10^(n+1) */

    /* initialise r = 33 */
    bi_set_uint(&r, 33);

    /* integer part of sqrt(33) is 5 */
    bi_set_uint(&p, 5);
    /* subtract 5*5 from remainder */
    bi_set_uint(&t, 25);
    bi_sub(&r, &t);  /* r = 33 - 25 = 8 */

    /* produce n+1 decimal digits of sqrt(33) */
    for (idx_t i = 0; i <= n; ++i) {
        /* bring down next pair of zeros: r = r * 100 */
        bi_mul_small(&r, 100);

        /* q = p * 20 */
        bi_copy(&q, &p);
        bi_mul_small(&q, 20);

        /* find largest m (0..9) with (q+m)*m <= r */
        int chosen = 0;
        for (int m = 9; m >= 0; --m) {
            bi_copy(&t, &q);          /* t = q */
            bi_add_small(&t, (uint32_t)m); /* t = q + m */
            bi_mul_small(&t, (uint32_t)m); /* t = (q+m)*m */
            if (bi_cmp(&t, &r) <= 0) {
                chosen = m;
                break;
            }
        }

        /* update p = p*10 + chosen */
        bi_mul_small(&p, 10);
        bi_add_small(&p, (uint32_t)chosen);

        /* update r = r - (q+chosen)*chosen */
        bi_copy(&t, &q);
        bi_add_small(&t, (uint32_t)chosen);
        bi_mul_small(&t, (uint32_t)chosen);
        bi_sub(&r, &t);
    }

    /* D = 3 * 10^(n+1) */
    bi_set_uint(&d, 3);
    for (idx_t i = 0; i <= n; ++i) {
        bi_mul_small(&d, 10);
    }

    /* a - D */
    bi_sub(&p, &d);

    /* divide by 8: p = floor((a-D)/8) */
    bi_div_small(&p, 8);

    /* last decimal digit of the quotient */
    uint32_t digit = bi_mod_small(&p, 10);

    /* free big ints */
    bi_free(&p);
    bi_free(&r);
    bi_free(&q);
    bi_free(&t);
    bi_free(&d);

    return (seq_t)digit;
}
