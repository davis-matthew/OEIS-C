/*
 * A353049  Decimal expansion of 8*sqrt(2) / 3.
 * https://oeis.org/A353049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A353049: Decimal expansion of 8*sqrt(2)/3.
   Formula: a(n) = floor( (8*sqrt(2)/3) * 10^(n-1) ) % 10
*/
#include <stdint.h>
#include <stddef.h>

/* Big integer in base 1e9, enough for >10 000 decimal digits */
#define BASE 1000000000U
#define MAX_LIMBS 1200

typedef struct {
    size_t len;               /* number of used limbs (len==0 means zero) */
    uint32_t d[MAX_LIMBS];    /* little-endian limbs, d[0] is lowest limb */
} BigInt;

/* Normalize by removing leading zero limbs */
static void bi_normalize(BigInt *n) {
    while (n->len > 0 && n->d[n->len - 1] == 0) n->len--;
}

/* Set big integer to zero */
static void bi_zero(BigInt *n) { n->len = 0; }

/* Set big integer to unsigned value v */
static void bi_set_uint(BigInt *n, uint32_t v) {
    if (v == 0) { n->len = 0; return; }
    n->d[0] = v;
    n->len = 1;
    bi_normalize(n);
}

/* Copy big integer */
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; i++) dst->d[i] = src->d[i];
}

/* Compare A and B: return -1 if A<B, 0 if A==B, 1 if A>B */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* Multiply big integer by small integer m (m <= 100) */
static void bi_mul_small(BigInt *n, uint32_t m) {
    if (m == 0 || n->len == 0) { n->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < n->len; i++) {
        uint64_t cur = (uint64_t)n->d[i] * m + carry;
        n->d[i]     = (uint32_t)(cur % BASE);
        carry       = cur / BASE;
    }
    while (carry) {
        n->d[n->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Add small integer m (m < BASE) to big integer */
static void bi_add_small(BigInt *n, uint32_t m) {
    uint64_t carry = m;
    for (size_t i = 0; i < n->len && carry; i++) {
        uint64_t cur = (uint64_t)n->d[i] + carry;
        n->d[i] = (uint32_t)(cur % BASE);
        carry   = cur / BASE;
    }
    if (carry) {
        n->d[n->len++] = (uint32_t)carry;
    }
}

/* Subtract big integer b from a (a >= b) */
static void bi_sub_big(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    size_t i = 0;
    for (; i < b->len || borrow; i++) {
        int64_t cur = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (cur < 0) {
            cur += BASE;
            borrow = 1;
        } else borrow = 0;
        a->d[i] = (uint32_t)cur;
    }
    bi_normalize(a);
}

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset is 1, but return 0 for safety */

    idx_t k = n - 1;                  /* groups count = k + 2 */
    size_t groups = k + 2;

    BigInt X, R, P, B, C;            /* temporary big integers */
    bi_zero(&X); bi_zero(&R); bi_zero(&P);
    bi_zero(&B); bi_zero(&C);

    for (size_t i = 0; i < groups; i++) {
        /* Determine next 2-digit group of the number 128*10^(2k) */
        uint32_t group;
        if (i == 0)      group = 1;   /* first group is '1' */
        else if (i == 1) group = 28;  /* second group is '28' */
        else            group = 0;    /* remaining groups are '00' */

        /* R = R*100 + group */
        bi_mul_small(&R, 100);
        if (group) bi_add_small(&R, group);

        /* P = X*20 */
        bi_copy(&P, &X);
        bi_mul_small(&P, 20);

        /* Find largest d (0..9) such that (20*X + d)*d <= R */
        int chosen = 0;
        for (int d = 9; d >= 0; d--) {
            bi_copy(&B, &P);
            bi_add_small(&B, (uint32_t)d);      /* B = 20*X + d */
            bi_copy(&C, &B);
            bi_mul_small(&C, (uint32_t)d);      /* C = (20*X + d)*d */

            if (bi_cmp(&C, &R) <= 0) {
                chosen = d;
                bi_sub_big(&R, &C);             /* R -= C */
                break;
            }
        }

        /* Update X = X*10 + chosen */
        bi_mul_small(&X, 10);
        bi_add_small(&X, (uint32_t)chosen);
    }

    /* Compute X mod 30, then divide by 3 to get last decimal digit */
    uint32_t rem = 0;
    for (size_t i = X.len; i-- > 0; ) {
        rem = (uint32_t)(((uint64_t)rem * BASE + X.d[i]) % 30);
    }
    uint32_t digit = rem / 3;
    return (seq_t)digit;
}
