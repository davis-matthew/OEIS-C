/*
 * A344382  Decimal expansion of sqrt(29)/5.
 * https://oeis.org/A344382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: decimal digits of sqrt(29)/5 via digit‑by‑digit
   sqrt extraction (base 10) and division by 5.  The sequence
   a(1) is the integer part (1), a(2) the first decimal digit,
   etc. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Big integer helpers (base 1e9) ---- */
typedef struct {
    size_t sz;       /* number of limbs used */
    size_t cap;      /* allocated capacity */
    uint32_t *v;     /* limbs, least significant first */
} BigInt;

#define BIGBASE 1000000000U

static void big_init(BigInt *a) { a->sz = 0; a->cap = 0; a->v = NULL; }

static void big_free(BigInt *a) { free(a->v); a->sz = 0; a->cap = 0; a->v = NULL; }

static void big_expand(BigInt *a, size_t need) {
    if (a->cap < need) {
        size_t new_cap = need;
        uint32_t *new_v = (uint32_t *)realloc(a->v, new_cap * sizeof(uint32_t));
        if (!new_v) { perror("realloc"); exit(1); }
        a->v = new_v; a->cap = new_cap;
    }
}

static void big_norm(BigInt *a) {
    while (a->sz > 0 && a->v[a->sz-1] == 0) a->sz--;
    if (a->sz == 0) { a->sz = 1; a->v[0] = 0; }
}

static void big_from_uint64(BigInt *a, uint64_t val) {
    big_expand(a, 1);
    a->sz = 0;
    do {
        a->v[a->sz++] = (uint32_t)(val % BIGBASE);
        val /= BIGBASE;
    } while (val);
    big_norm(a);
}

static void big_copy(BigInt *dst, const BigInt *src) {
    big_expand(dst, src->sz);
    memcpy(dst->v, src->v, src->sz * sizeof(uint32_t));
    dst->sz = src->sz;
}

static void big_mul_small(BigInt *a, unsigned int m) {
    if (m == 0) { a->sz = 1; a->v[0] = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        uint64_t prod = (uint64_t)a->v[i] * m + carry;
        a->v[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    while (carry) {
        big_expand(a, a->sz + 1);
        a->v[a->sz++] = (uint32_t)carry;
        carry /= BIGBASE;
    }
    big_norm(a);
}

static void big_add_small(BigInt *a, unsigned int add) {
    uint64_t sum = (uint64_t)a->v[0] + add;
    a->v[0] = (uint32_t)(sum % BIGBASE);
    uint64_t carry = sum / BIGBASE;
    size_t i = 1;
    while (carry) {
        if (i >= a->sz) {
            big_expand(a, a->sz + 1);
            a->sz++; a->v[i] = 0;
        }
        uint64_t s = (uint64_t)a->v[i] + carry;
        a->v[i] = (uint32_t)(s % BIGBASE);
        carry = s / BIGBASE;
        ++i;
    }
}

static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->sz != b->sz) return (a->sz < b->sz) ? -1 : 1;
    for (size_t i = 0; i < a->sz; ++i) {
        size_t ia = a->sz - 1 - i, ib = b->sz - 1 - i;
        if (a->v[ia] != b->v[ib])
            return (a->v[ia] < b->v[ib]) ? -1 : 1;
    }
    return 0;
}

static void big_sub(BigInt *a, const BigInt *b) {          /* assume a >= b */
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        uint64_t ai = a->v[i];
        uint64_t bi = (i < b->sz) ? b->v[i] : 0;
        uint64_t sub = bi + borrow;
        if (ai >= sub) {
            a->v[i] = (uint32_t)(ai - sub);
            borrow = 0;
        } else {
            a->v[i] = (uint32_t)(ai + BIGBASE - sub);
            borrow = 1;
        }
    }
    big_norm(a);
}

/* ---- Algorithm state ---- */
static BigInt P, R, tmp1, tmp2;          /* for the sqrt extraction */
static uint8_t *div_digits = NULL;       /* decimal digits of sqrt(29)/5 */
static size_t div_cap = 0;               /* capacity of div_digits */
static size_t div_used = 0;              /* number of digits stored */
static bool int_done = false;            /* processed integer part of sqrt(29) */
static unsigned int r_div = 0;           /* remainder for division by 5 */

static void div_ensure(size_t need) {
    if (need > div_cap) {
        size_t new_cap = div_cap ? div_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        uint8_t *newv = (uint8_t *)realloc(div_digits, new_cap);
        if (!newv) { perror("realloc"); exit(1); }
        div_digits = newv; div_cap = new_cap;
    }
}

static void init_state(void) {
    big_init(&P); big_from_uint64(&P, 0);
    big_init(&R); big_from_uint64(&R, 0);
    big_init(&tmp1); big_from_uint64(&tmp1, 0);
    big_init(&tmp2); big_from_uint64(&tmp2, 0);
    div_digits = NULL; div_cap = 0; div_used = 0;
    int_done = false; r_div = 0;
}

static void compute_next_sqrt_digit(void) {
    static bool first_group = true;
    unsigned int group = first_group ? 29 : 0;
    first_group = false;

    big_mul_small(&R, 100);
    big_add_small(&R, group);

    big_copy(&tmp1, &P);
    big_mul_small(&tmp1, 20);

    unsigned int d = 0;
    for (int dd = 9; dd >= 0; --dd) {
        big_copy(&tmp2, &tmp1);
        big_add_small(&tmp2, (unsigned int)dd);
        big_mul_small(&tmp2, (unsigned int)dd);
        if (big_cmp(&tmp2, &R) <= 0) { d = (unsigned int)dd; break; }
    }

    big_sub(&R, &tmp2);

    big_mul_small(&P, 10);
    big_add_small(&P, d);

    if (!int_done) {
        /* integer part (first digit 5) */
        uint8_t q = d / 5;          /* integer quotient = 1 */
        r_div = d % 5;              /* remainder 0 */
        div_ensure(1);
        div_digits[0] = q;
        div_used = 1;
        int_done = true;
    } else {
        r_div = r_div * 10 + d;
        uint8_t q = (uint8_t)(r_div / 5);
        r_div %= 5;
        div_ensure(div_used + 1);
        div_digits[div_used++] = q;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n) {
    if (!div_digits) init_state();

    while (div_used < (size_t)n) compute_next_sqrt_digit();

    return (seq_t)div_digits[n-1];
}
