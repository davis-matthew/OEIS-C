/*
 * A333567  Decimal expansion of the continued fraction expansion [1; 3, 4, 7,
 *          11, 18,...] (the Lucas numbers beginning at 1, A000204).
 * https://oeis.org/A333567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th digit in the decimal expansion of the continued fraction
   [1; 3, 4, 7, 11, 18, …] (Lucas numbers starting at 1).
   a(1) = 1, a(2) = 3, a(3) = 0, a(4) = 8, …  (OEIS A333567)   */

#include <stdint.h>
#include <stdlib.h>

/* Big integer in base 10^9, little‑endian */
typedef struct {
    uint32_t *dp;  /* digits, dp[0] least significant */
    size_t    len; /* number of used words */
} Big;

#define BASE 1000000000U

/* ------------------------------------------------------------------ */
/* Basic Big integer helpers                                         */
/* ------------------------------------------------------------------ */

static void big_init(Big *b) { b->dp = NULL; b->len = 0; }

static void big_free(Big *b)
{
    if (b->dp) free(b->dp);
    b->dp = NULL;
    b->len = 0;
}

static void big_copy(const Big *src, Big *dst)
{
    if (src->len == 0) {
        big_free(dst);
        return;
    }
    dst->dp = (uint32_t *)malloc(src->len * sizeof(uint32_t));
    memcpy(dst->dp, src->dp, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void big_norm(Big *b)
{
    while (b->len && b->dp[b->len - 1] == 0) b->len--;
    if (b->len == 0 && b->dp) {
        free(b->dp);
        b->dp = NULL;
    }
}

static void big_from_uint64(Big *b, uint64_t v)
{
    big_free(b);
    if (v == 0) return;
    b->len = 0;
    b->dp = NULL;
    while (v) {
        b->dp = (uint32_t *)realloc(b->dp, ++b->len * sizeof(uint32_t));
        b->dp[b->len - 1] = (uint32_t)(v % BASE);
        v /= BASE;
    }
}

static int big_cmp(const Big *a, const Big *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->dp[i] != b->dp[i]) return (a->dp[i] > b->dp[i]) ? 1 : -1;
    }
    return 0;
}

static void big_add(const Big *a, const Big *b, Big *res)
{
    size_t l = (a->len > b->len) ? a->len : b->len;
    res->dp = (uint32_t *)malloc((l + 1) * sizeof(uint32_t));
    uint64_t carry = 0;
    for (size_t i = 0; i < l; ++i) {
        uint64_t aa = (i < a->len) ? a->dp[i] : 0;
        uint64_t bb = (i < b->len) ? b->dp[i] : 0;
        uint64_t sum = aa + bb + carry;
        res->dp[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        res->dp[l] = (uint32_t)carry;
        res->len = l + 1;
    } else {
        res->len = l;
    }
    big_norm(res);
}

static void big_sub(const Big *a, const Big *b, Big *res)
{
    /* assume a >= b */
    res->dp = (uint32_t *)malloc(a->len * sizeof(uint32_t));
    int64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t aa = a->dp[i];
        int64_t bb = (i < b->len) ? b->dp[i] : 0;
        int64_t diff = aa - bb + carry;
        if (diff < 0) {
            diff += BASE;
            carry = -1;
        } else {
            carry = 0;
        }
        res->dp[i] = (uint32_t)diff;
    }
    res->len = a->len;
    big_norm(res);
}

static void big_mul_uint32(const Big *a, uint32_t m, Big *res)
{
    if (m == 0 || a->len == 0) {
        big_free(res);
        return;
    }
    res->dp = (uint32_t *)malloc((a->len + 1) * sizeof(uint32_t));
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->dp[i] * m + carry;
        res->dp[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        res->dp[a->len] = (uint32_t)carry;
        res->len = a->len + 1;
    } else {
        res->len = a->len;
    }
    big_norm(res);
}

static void big_mul(const Big *a, const Big *b, Big *res)
{
    if (a->len == 0 || b->len == 0) {
        big_free(res);
        return;
    }
    res->dp = (uint32_t *)calloc(a->len + b->len, sizeof(uint32_t));
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b->len; ++j) {
            uint64_t cur = res->dp[i + j] + (uint64_t)a->dp[i] * b->dp[j] + carry;
            res->dp[i + j] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        res->dp[i + b->len] += (uint32_t)carry;
    }
    res->len = a->len + b->len;
    big_norm(res);
}

/* number of decimal digits of a big integer */
static int big_decimal_len(const Big *b)
{
    if (b->len == 0) return 1;
    int high = b->dp[b->len - 1];
    int digits;
    if (high >= 100000000U) digits = 9;
    else if (high >= 10000000U) digits = 8;
    else if (high >= 1000000U) digits = 7;
    else if (high >= 100000U) digits = 6;
    else if (high >= 10000U) digits = 5;
    else if (high >= 1000U) digits = 4;
    else if (high >= 100U) digits = 3;
    else if (high >= 10U) digits = 2;
    else digits = 1;
    return (b->len - 1) * 9 + digits;
}

/* ------------------------------------------------------------------ */
/* Core function                                                      */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 */

    /* ------------------------------------------------------------------
       1. Build convergents until denominator has > n decimal digits
           i.e. q > 10^n
       ------------------------------------------------------------------ */

    Big p_prevprev, p_prev, p_next;
    Big q_prevprev, q_prev, q_next;
    big_init(&p_prevprev); big_init(&p_prev); big_init(&p_next);
    big_init(&q_prevprev); big_init(&q_prev); big_init(&q_next);

    big_from_uint64(&p_prevprev, 0);     /* p_-2 = 0 */
    big_from_uint64(&p_prev,    1);      /* p_-1 = 1 */
    big_from_uint64(&q_prevprev, 1);     /* q_-2 = 1 */
    big_from_uint64(&q_prev,    0);      /* q_-1 = 0 */

    /* Lucas numbers L0=2, L1=1 */
    Big luc_prev, luc_curr, luc_next;
    big_init(&luc_prev); big_init(&luc_curr); big_init(&luc_next);
    big_from_uint64(&luc_prev, 2);  /* L0 */
    big_from_uint64(&luc_curr, 1);  /* L1 */

    idx_t idx = 0;  /* current partial quotient index (a0 = 1) */

    while (1) {
        /* current partial quotient a_k */
        Big a_k;
        big_init(&a_k);
        if (idx == 0) {
            big_from_uint64(&a_k, 1);   /* a0 = 1 */
        } else {
            /* compute next Lucas number L_{idx+1} */
            big_add(&luc_prev, &luc_curr, &luc_next);
            big_copy(&luc_next, &a_k);
            /* rotate luc numbers for next iteration */
            big_free(&luc_prev); big_copy(&luc_curr, &luc_prev);
            big_free(&luc_curr); big_copy(&luc_next, &luc_curr);
        }

        /* next convergents */
        Big tmp;
        big_mul(&a_k, &p_prev, &tmp);
        big_add(&tmp, &p_prevprev, &p_next);
        big_free(&tmp);

        big_mul(&a_k, &q_prev, &tmp);
        big_add(&tmp, &q_prevprev, &q_next);
        big_free(&tmp);

        /* shift for next iteration */
        big_free(&p_prevprev); big_copy(&p_prev, &p_prevprev);
        big_free(&p_prev);     big_copy(&p_next, &p_prev);
        big_free(&q_prevprev); big_copy(&q_prev, &q_prevprev);
        big_free(&q_prev);     big_copy(&q_next, &q_prev);

        big_free(&a_k);

        idx++;

        if (big_decimal_len(&q_prev) > n) break;
    }

    /* ------------------------------------------------------------------
       2. Integer part and first digit (position 1)
       ------------------------------------------------------------------ */
    /* find integer part (p_prev / q_prev) */
    seq_t int_part = 0;
    for (int d = 9; d >= 0; --d) {
        Big prod;
        big_mul_uint32(&q_prev, (uint32_t)d, &prod);
        if (big_cmp(&p_prev, &prod) >= 0) {
            int_part = (seq_t)d;
            big_free(&prod);
            break;
        }
        big_free(&prod);
    }
    if (n == 1) {
        /* free temporary big objects before returning */
        big_free(&p_prevprev); big_free(&p_prev);
        big_free(&q_prevprev); big_free(&q_prev);
        big_free(&luc_prev); big_free(&luc_curr); big_free(&luc_next);
        return int_part;
    }

    /* remainder after integer part */
    Big remainder;
    Big int_q;
    big_mul_uint32(&q_prev, (uint32_t)int_part, &int_q);
    big_sub(&p_prev, &int_q, &remainder);
    big_free(&int_q);

    /* ------------------------------------------------------------------
       3. Generate digits up to position n
       ------------------------------------------------------------------ */
    seq_t digit = 0;
    Big r_tmp, mult_q, cmp_res, r10, prod;
    for (idx_t pos = 2; pos <= n; ++pos) {
        /* r = r * 10 */
        big_mul_uint32(&remainder, 10, &r10);

        /* binary search for digit d */
        int lo = 0, hi = 9;
        int best = 0;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            big_mul_uint32(&q_prev, (uint32_t)mid, &mult_q);
            int cmp = big_cmp(&mult_q, &r10);
            if (cmp <= 0) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
            big_free(&mult_q);
        }
        digit = (seq_t)best;

        /* remainder = r10 - best * q_prev */
        big_mul_uint32(&q_prev, (uint32_t)best, &prod);
        big_sub(&r10, &prod, &remainder);
        big_free(&prod);
        big_free(&r10);

        if (pos == n) {
            /* clean up */
            big_free(&p_prevprev); big_free(&p_prev);
            big_free(&q_prevprev); big_free(&q_prev);
            big_free(&luc_prev); big_free(&luc_curr); big_free(&luc_next);
            return digit;
        }
    }

    /* should not reach here */
    return 0;
}
