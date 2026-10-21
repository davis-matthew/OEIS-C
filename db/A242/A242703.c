/*
 * A242703  Decimal expansion of sqrt(7)/2.
 * https://oeis.org/A242703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, cons, nonn
 * Formula:   a(n) = floor((sqrt(7)/2) * 10^(n-1)) mod 10
 * Range:     u8 n <= 9438, u16 n <= 9438, u32 n <= 9438, u64 n <= 9438,
 *            u128 n <= 9438, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the nth decimal digit of sqrt(7)/2 (a(1) is the integer part). */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BASE 1000000000U          /* fits in 32‑bit, product fits in 64‑bit */
#define MAX_DIGITS 2000           /* sufficient for n up to ~18000 */

typedef struct {
    size_t len;               /* number of limbs used (little‑endian) */
    uint32_t d[MAX_DIGITS];
} BigInt;

static void bi_zero(BigInt *a) {
    a->len = 1;
    a->d[0] = 0;
}

static void bi_set_ui(BigInt *a, uint32_t v) {
    a->len = 1;
    a->d[0] = v;
}

static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static void bi_trim(BigInt *a) {
    while (a->len > 1 && a->d[a->len - 1] == 0)
        a->len--;
}

/* multiply *a* by small integer *m* (m < BASE) and store result */
static void bi_mul_small(const BigInt *a, uint32_t m, BigInt *dst) {
    uint64_t carry = 0;
    dst->len = a->len;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        dst->d[i] = (uint32_t)(prod % BASE);
        carry    = prod / BASE;
    }
    if (carry) {
        dst->d[dst->len++] = (uint32_t)carry;
    }
}

static void bi_add_small_inplace(BigInt *a, uint32_t v) {
    uint64_t sum = (uint64_t)a->d[0] + v;
    a->d[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* a = a * 10 (in-place) */
static void bi_mul10_inplace(BigInt *a) {
    bi_mul_small(a, 10, a);
}

/* a = a * 100 (in-place) */
static void bi_mul100_inplace(BigInt *a) {
    bi_mul_small(a, 100, a);
}

/* result = a - b   (assumes a >= b) */
static void bi_sub(BigInt *result, const BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    result->len = a->len;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->d[i] = (uint32_t)sub;
    }
    bi_trim(result);
}

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* outside the offset – not used */

    BigInt result, remainder, temp, candidate;
    bi_zero(&result);
    bi_zero(&remainder);

    int res_mod20 = 0;            /* keeps result mod 20 */

    for (idx_t step = 0; step < n; ++step) {
        /* bring down the next pair of digits (first step brings 7, others bring 00) */
        bi_mul100_inplace(&remainder);
        bi_add_small_inplace(&remainder, (step == 0) ? 7 : 0);

        /* find largest d (0..9) such that (20*result + d)*d <= remainder */
        int chosen = 0;
        for (int d = 9; d >= 0; --d) {
            /* temp = 20*result */
            bi_mul_small(&result, 20, &temp);
            /* temp += d */
            bi_add_small_inplace(&temp, (uint32_t)d);
            /* candidate = temp * d */
            bi_mul_small(&temp, (uint32_t)d, &candidate);
            if (bi_cmp(&candidate, &remainder) <= 0) {
                chosen = d;
                break;
            }
        }

        /* update result: result = result*10 + chosen */
        bi_mul10_inplace(&result);
        bi_add_small_inplace(&result, (uint32_t)chosen);

        /* remainder = remainder - candidate */
        bi_sub(&remainder, &remainder, &candidate);

        /* update result modulo 20 for final digit computation */
        res_mod20 = (res_mod20 * 10 + chosen) % 20;
    }

    /* a(n) = floor((result/2)) % 10, i.e. (res_mod20/2) % 10 */
    uint32_t digit = (res_mod20 / 2) % 10;
    return (seq_t)digit;
}
