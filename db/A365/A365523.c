/*
 * A365523  Decimal expansion of 6*log(2) - 4.
 * https://oeis.org/A365523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit n of 6*log(2) - 4 Method: compute
 *            (6*log(2)-4) to B bits, then extract decimal digits by repeated
 *            multiply by 10 and dividing by 2^B
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- big‑integer utilities (base 2^32) ---------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* maximum bits needed for 10001 decimal digits
   10001 * log2(10)  ≈ 33277, plus a few safety bits    */
#define MAX_WORDS   1500    /* 1500 * 32 = 48000 bits, enough */

/* Big integer: little‑endian word array */
typedef struct {
    size_t len;            /* number of used 32‑bit words */
    uint32_t w[MAX_WORDS];
} BigInt;

static void bi_zero(BigInt *a) { a->len = 0; }
static void bi_init(BigInt *a, uint64_t v)
{ bi_zero(a); while (v) { a->w[a->len++] = (uint32_t)v; v >>= 32; } }

static void bi_normalize(BigInt *a)
{ while (a->len && a->w[a->len-1]==0) a->len--; }

static void bi_copy(BigInt *dst, const BigInt *src)
{ dst->len = src->len; memcpy(dst->w, src->w, src->len * sizeof(uint32_t)); }

static void bi_add(BigInt *res, const BigInt *a, const BigInt *b)
{
    size_t n = res->len > a->len ? res->len : a->len;
    if (n < b->len) n = b->len;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < n || carry; ++i) {
        uint64_t va = (i < a->len) ? a->w[i] : 0;
        uint64_t vb = (i < b->len) ? b->w[i] : 0;
        uint64_t sum = va + vb + carry;
        if (i < res->len) res->w[i] = (uint32_t)sum;
        else res->w[res->len++] = (uint32_t)sum;
        carry = sum >> 32;
    }
    bi_normalize(res);
}

static void bi_mul_small(BigInt *a, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        uint64_t v = (uint64_t)a->w[i] * m + carry;
        a->w[i] = (uint32_t)v;
        carry = v >> 32;
    }
    if (carry) a->w[a->len++] = (uint32_t)carry;
}

static void bi_div_small(BigInt *a, uint32_t d, uint32_t *rem)
{
    uint64_t r = 0;
    size_t i;
    for (i = a->len; i-- > 0;) {
        r = (r << 32) | a->w[i];
        a->w[i] = (uint32_t)(r / d);
        r = r % d;
    }
    if (rem) *rem = (uint32_t)r;
    bi_normalize(a);
}

static void bi_sub(BigInt *res, const BigInt *a, const BigInt *b)
{
    /* res = a - b,  assume a >= b */
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        uint64_t sub = borrow;
        if (i < b->len) sub += b->w[i];
        if (a->w[i] < sub) {
            res->w[i] = (uint32_t)(0x100000000ULL + a->w[i] - sub);
            borrow = 1;
        } else {
            res->w[i] = (uint32_t)(a->w[i] - sub);
            borrow = 0;
        }
    }
    bi_normalize(res);
}

static void bi_shift_right_one(BigInt *a)
{
    uint32_t carry = 0;
    size_t i;
    for (i = a->len; i-- > 0;) {
        uint32_t newcarry = a->w[i] & 1;
        a->w[i] = (a->w[i] >> 1) | (carry << 31);
        carry = newcarry;
    }
    bi_normalize(a);
}

/* ---------- sequence computation ---------- */

static seq_t *digit_cache = NULL;    /* cache of digits 0 … maximum required */
static idx_t cached_max = 0;         /* last computed size */
static const int MAX_REQUIRED = 10000;   /* generate up to this many digits */

/* build table of decimal digits up to max_n (inclusive) */
static void build_cache(idx_t max_n)
{
    if (digit_cache && cached_max >= max_n) return;   /* already done */

    if (digit_cache) { free(digit_cache); digit_cache = NULL; }
    cached_max = max_n;
    digit_cache = (seq_t*)malloc(sizeof(seq_t) * (max_n + 1));
    if (!digit_cache) return;   /* out of memory – caller must handle but
                                   unlikely in test harness */

    /* compute B, number of bits required */
    const double LOG2_10 = 3.3219280948873623478703194294894;
    int d = (int)max_n + 5;          /* a few extra decimal digits */
    int B = (int)ceil(d * LOG2_10) + 5;   /* safety margin */

    int K = B;    /* number of terms needed in series */

    /* big integers used */
    BigInt sum, pow2, term, fourPow2;
    bi_zero(&sum);
    bi_zero(&pow2);
    bi_zero(&fourPow2);

    /* pow2 = 2^(B-1)  (first term's power of two) */
    int shift = B - 1;
    int widx = shift / 32;
    int bidx = shift % 32;
    pow2.w[widx] = 1u << bidx;
    pow2.len = widx + 1;

    /* 4 << B for later subtraction */
    int w4 = B / 32;
    int beg4 = B % 32;
    fourPow2.w[w4] = 4u << beg4;
    fourPow2.len = w4 + 1;

    /* compute the series Σ 6/(k*2^k) * 2^B */
    for (int k = 1; k <= K; ++k) {
        /* term = pow2 */
        bi_copy(&term, &pow2);

        /* *6 */
        bi_mul_small(&term, 6u);

        /* /k  (small integer) */
        bi_div_small(&term, (uint32_t)k, NULL);

        /* add to running sum */
        bi_add(&sum, &sum, &term);

        /* next pow2: divide previous by 2 */
        bi_shift_right_one(&pow2);
        if (pow2.len == 0) break;   /* further terms are zero */
    }

    /* subtract 4*2^B  (i.e. fourPow2) from sum */
    bi_sub(&sum, &sum, &fourPow2);

    /* digit extraction: cur holds the scaled integer */
    BigInt cur;
    bi_copy(&cur, &sum);

    int wordShift = B / 32;
    int bitShift  = B % 32;
    uint32_t maskLow = (bitShift == 0) ? 0xFFFFFFFFu : ((1u << bitShift) - 1);

    for (idx_t i = 0; i <= (idx_t)max_n; ++i) {
        /* multiply current value by 10 */
        bi_mul_small(&cur, 10u);

        /* determine the next decimal digit */
        uint64_t low = 0, high = 0;
        if (wordShift < cur.len) {
            low  = cur.w[wordShift];
            high = (wordShift + 1 < cur.len) ? cur.w[wordShift + 1] : 0;
        }
        uint64_t top = (high << 32) | low;
        uint32_t digit = (uint32_t)(top >> bitShift);
        digit_cache[i] = (seq_t)digit;

        /* keep only the lower B bits for the next step */
        size_t newlen = wordShift;
        if (bitShift && wordShift < cur.len) newlen++;   /* extra word for low bits */

        size_t j;
        for (j = 0; j < newlen && j < cur.len; ++j)
            cur.w[j] = cur.w[j];
        cur.len = newlen;
        if (bitShift && wordShift < cur.len) {
            cur.w[wordShift] &= maskLow;
        }
        bi_normalize(&cur);
    }
}

/* ---------- exported sequence function ---------- */

seq_t a(idx_t n)
{
    if (!digit_cache || cached_max < n) build_cache((idx_t)MAX_REQUIRED);
    if (n <= cached_max) return digit_cache[n];
    return (seq_t)0;   /* fallback – should not be needed for test data */
}
