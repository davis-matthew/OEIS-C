/*
 * A364895  Decimal expansion of the 4-volume of the unit regular pentachoron
 *          (5-cell).
 * https://oeis.org/A364895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((sqrt(5)/96) * 10^(n+1)) mod 10
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Custom arbitrary‑precision integer (little‑endian, base 1e9) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000U

typedef struct {
    size_t len;
    uint32_t *d;          /* little‑endian digits, 0 <= d[i] < BASE */
} BigInt;

static void bi_init(BigInt *a, size_t cap) {
    a->len = 0;
    a->d = (uint32_t *)malloc(cap * sizeof(uint32_t));
}

static void bi_free(BigInt *a) {
    free(a->d);
    a->len = 0;
}

static void bi_trim(BigInt *a) {
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return a->len > b->len ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i] ? 1 : -1;
    }
    return 0;
}

static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static void bi_add_small(BigInt *a, uint32_t m) {
    uint64_t carry = m;
    for (size_t i = 0; i < a->len || carry; ++i) {
        if (i == a->len) {
            a->d[i] = (uint32_t)(carry % BASE);
            carry /= BASE;
            ++a->len;
        } else {
            uint64_t sum = (uint64_t)a->d[i] + carry;
            a->d[i] = (uint32_t)(sum % BASE);
            carry = sum / BASE;
        }
    }
}

static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 1 || a->len == 0) return;
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

static void bi_sub_assign(BigInt *a, const BigInt *b) { /* assumes a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    bi_trim(a);
}

static uint32_t bi_mod_small_uint(const BigInt *a, uint32_t mod) {
    uint64_t r = 0;
    for (size_t i = a->len; i-- > 0; ) {
        r = (r * BASE + a->d[i]) % mod;
    }
    return (uint32_t)r;
}

/* The sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t k = n + 1;                     /* exponent of 10 */

    /* capacity for digits in base 1e9 */
    size_t cap = (k + 9) / 9 + 4;

    BigInt result, rem, cand, temp;
    bi_init(&result, cap);
    bi_init(&rem, cap);
    bi_init(&cand, cap);
    bi_init(&temp, cap);

    for (idx_t i = 0; i <= k; ++i) {
        uint32_t next_pair = (i == 0) ? 5 : 0;   /* first pair is 05, others 00 */

        /* remainder = remainder*100 + next_pair */
        bi_mul_small(&rem, 100);
        bi_add_small(&rem, next_pair);

        /* precompute cand = result*20 once */
        bi_copy(&cand, &result);
        bi_mul_small(&cand, 20);

        /* find largest d in [0,9] with (cand+d)*d <= rem */
        uint32_t d = 0;
        for (d = 0; d <= 9; ++d) {
            bi_copy(&temp, &cand);                /* temp = cand */
            bi_add_small(&temp, d);               /* temp += d */
            bi_mul_small(&temp, d);               /* temp *= d */
            if (bi_cmp(&temp, &rem) > 0) {        /* too big */
                if (d == 0) d = 0;                /* stay at 0 */
                else d--;                         /* last valid */
                break;
            }
            if (d == 9) break;                    /* reached 9 with valid */
        }
        if (d > 9) d = 9;                         /* safety */

        /* compute product for final d and subtract */
        bi_copy(&temp, &cand);
        bi_add_small(&temp, d);
        bi_mul_small(&temp, d);
        bi_sub_assign(&rem, &temp);

        /* result = result*10 + d */
        bi_mul_small(&result, 10);
        bi_add_small(&result, d);
    }

    /* digit = floor((result / 96)) % 10 = (result mod 960) / 96 */
    uint32_t modvalue = bi_mod_small_uint(&result, 960);
    seq_t digit = modvalue / 96;

    bi_free(&result);
    bi_free(&rem);
    bi_free(&cand);
    bi_free(&temp);

    return digit;
}
