/*
 * A355328  Decimal expansion of the number whose binary expansion differs
 *          from its decimal expansion only in the first digit.
 * https://oeis.org/A355328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(0)=1. For n≥1 compute a(n) by greedy selection of bits to
   represent 1/10 as a sum of weights
       w_k = 1/2^{k+1} - 1/10^{k+1}   (k≥1).
   The digits a(n) are the indicators that the corresponding weight is
   used.  The computation uses an arbitrary‑precision integer to
   handle the growing numerators. */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Basic big integer using base 2^32 limbs */
#define BIGLEN 2000

typedef struct {
    uint32_t d[BIGLEN];
    size_t len;                 /* number of used limbs (≥1) */
} Big;

/* set Big to unsigned integer v */
static void big_set_uint(Big *a, uint32_t v) {
    a->len = 1;
    a->d[0] = v;
}

/* copy Big b into a */
static void big_copy(Big *a, const Big *b) {
    a->len = b->len;
    memcpy(a->d, b->d, a->len * sizeof(uint32_t));
}

/* compare two Big numbers: return -1,0,1 */
static int big_cmp(const Big *a, const Big *b) {
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* a = a - b   (assumes a ≥ b) */
static void big_sub(Big *a, const Big *b) {
    uint32_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint32_t ai = a->d[i];
        uint32_t bi = (i < b->len) ? b->d[i] : 0;
        uint32_t r = ai - bi - borrow;
        uint32_t sum_bi = bi + borrow;
        borrow = (ai < sum_bi) ? 1 : 0;
        a->d[i] = r;
    }
    while (a->len > 1 && a->d[a->len - 1] == 0)
        --a->len;
}

/* a = a * m   (m is small, here ≤ 20) */
static void big_mul_small(Big *a, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod & 0xffffffffu);
        carry = prod >> 32;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* cached results */
#define MAXDIG 20001
static seq_t digits[MAXDIG];
static idx_t digits_len = 1;                 /* digits[0] is a(0) */

/* big integers needed for the greedy algorithm */
static bool init_done = false;
static Big rem, pow10, pow2;

seq_t a(idx_t n)
{
    /* one‑time initialization */
    if (!init_done) {
        /* a(0) = 1 */
        digits[0] = 1;
        digits_len = 1;

        /* start with m = 2  (first weight uses k=1 => m=2) */
        big_set_uint(&pow10, 100);          /* 10^2 */
        big_set_uint(&pow2, 4);             /* 2^2  */
        big_set_uint(&rem, 2);              /* R_num = 2  (represents 2 / 20 = 1/10) */

        init_done = true;
    }

    if (n < digits_len)
        return digits[n];

    /* temporary Big integers */
    Big weight, temp;

    /* generate digits until we reach n */
    while (digits_len <= n) {

        /* weight = pow10 - pow2  ( == 10^m - 2^m ) */
        big_copy(&weight, &pow10);
        big_sub(&weight, &pow2);

        /* temp = rem * 20   (scale remainder for comparison) */
        big_copy(&temp, &rem);
        big_mul_small(&temp, 20);

        /* decide digit */
        if (big_cmp(&temp, &weight) >= 0) {
            digits[digits_len] = 1;
            big_sub(&temp, &weight);       /* temp = rem*20 - weight */
        } else {
            digits[digits_len] = 0;
        }

        /* update remainder to scaled value for next step */
        big_copy(&rem, &temp);

        /* advance to next weight: increase m by 1 */
        big_mul_small(&pow10, 10);          /* 10^m -> 10^{m+1} */
        big_mul_small(&pow2, 2);            /* 2^m  -> 2^{m+1} */

        ++digits_len;
    }

    return digits[n];
}
