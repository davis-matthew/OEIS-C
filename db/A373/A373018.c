/*
 * A373018  Decimal expansion of Sum_{k>=0} |sin(k*Pi/3)|/2^k.
 * https://oeis.org/A373018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   ensure_uint8_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit extraction for decimal expansion of (3/7)*sqrt(3) */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ensure_uint8_cap.h"

#define BIG_BASE 1000000000u   /* base for big‑integer limbs */

typedef struct {
    uint32_t *d;   /* digits, little‑endian, each < BIG_BASE */
    size_t len;
    size_t cap;
} BigInt;

static void bigint_init(BigInt *a) {
    a->len = 0;
    a->cap = 0;
    a->d   = NULL;
}

static void bigint_realloc_cap(BigInt *a, size_t newlen) {
    if (newlen > a->cap) {
        size_t newcap = (a->cap ? a->cap * 2 : 16);
        while (newcap < newlen) newcap *= 2;
        a->d = (uint32_t*)realloc(a->d, newcap * sizeof(uint32_t));
        a->cap = newcap;
    }
}

static void bigint_copy(BigInt *dest, const BigInt *src) {
    if (dest == src) return;
    bigint_realloc_cap(dest, src->len);
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
    dest->len = src->len;
}

static void bigint_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { a->len = 0; return; }
    unsigned long long carry = 0;
    size_t i;
    bigint_realloc_cap(a, a->len);
    for (i = 0; i < a->len; i++) {
        unsigned long long prod = (unsigned long long)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    while (carry) {
        bigint_realloc_cap(a, a->len + 1);
        a->d[a->len] = (uint32_t)(carry % BIG_BASE);
        carry       /= BIG_BASE;
        a->len++;
    }
}

static void bigint_add_small(BigInt *a, uint32_t v) {
    size_t i = 0;
    unsigned long long sum = v;
    while (sum) {
        if (i == a->len) {
            bigint_realloc_cap(a, a->len + 1);
            a->d[i] = 0;
            a->len++;
        }
        sum += a->d[i];
        a->d[i] = (uint32_t)(sum % BIG_BASE);
        sum /= BIG_BASE;
        i++;
    }
}

static int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i > 0; ) {
        i--;
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

static void bigint_sub_assign(BigInt *a, const BigInt *b) {
    int borrow = 0;
    size_t i = 0;
    while (i < a->len) {
        int64_t ai = a->d[i];
        int64_t bi = (i < b->len) ? b->d[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) { diff += BIG_BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)diff;
        i++;
    }
    while (a->len > 0 && a->d[a->len-1] == 0) a->len--;
}

static uint8_t *sqrt_digits = NULL;
static size_t sqrt_digits_len = 0, sqrt_digits_cap = 0;
static uint8_t *quot_digits = NULL;
static size_t quot_digits_len = 0, quot_digits_cap = 0;

static int first_group = 1;   /* first pair is 27, following pairs 00 */
static int div_rem    = 0;    /* remainder of division by 7, small int */

static BigInt rem_big;  /* remainder in sqrt extraction */
static BigInt root_big; /* current root, in decimal digits */
static BigInt base_big; /* root * 20 */
static BigInt cand_big; /* candidate product */

/* Ensure big integers are initialized once */
static int big_init_done = 0;
static void big_init(void) {
    if (big_init_done) return;
    big_init_done = 1;
    bigint_init(&rem_big);
    bigint_init(&root_big);
    bigint_init(&base_big);
    bigint_init(&cand_big);
}

/* Generate additional digits until sqrt_digits_len reaches target_digits;
   each step also generates one quotient digit (for division by 7) */
static void extend_digits(size_t target_digits) {
    big_init();
    while (sqrt_digits_len < target_digits) {
        /* bring down the next pair */
        bigint_mul_small(&rem_big, 100);
        bigint_add_small(&rem_big, first_group ? 27u : 0u);
        if (first_group) first_group = 0;

        /* base = root * 20 */
        bigint_copy(&base_big, &root_big);
        bigint_mul_small(&base_big, 20u);

        /* find largest x (0..9) with (base + x)*x <= rem */
        int x;
        for (x = 9; x >= 0; x--) {
            bigint_copy(&cand_big, &base_big);
            bigint_mul_small(&cand_big, (uint32_t)x);
            uint32_t add = (uint32_t)(x * x);
            bigint_add_small(&cand_big, add);
            if (bigint_cmp(&cand_big, &rem_big) <= 0) break;
        }

        /* update remainder and root */
        bigint_sub_assign(&rem_big, &cand_big);
        bigint_mul_small(&root_big, 10u);
        bigint_add_small(&root_big, (uint32_t)x);

        /* store the sqrt digit x */
        ensure_uint8_cap(&sqrt_digits, &sqrt_digits_len, &sqrt_digits_cap, sqrt_digits_len + 1);
        sqrt_digits[sqrt_digits_len++] = (uint8_t)x;

        /* perform one division step by 7 */
        div_rem = div_rem * 10 + x;
        int qdigit = div_rem / 7;
        div_rem %= 7;
        ensure_uint8_cap(&quot_digits, &quot_digits_len, &quot_digits_cap, quot_digits_len + 1);
        quot_digits[quot_digits_len++] = (uint8_t)qdigit;
    }
}

/* Required sequence function */
seq_t a(idx_t n) {
    size_t target_digits = (size_t)n + 2;   /* need n+2 quotient digits (incl. leading 0) */
    extend_digits(target_digits);
    return (seq_t)quot_digits[1 + (size_t)n];
}
