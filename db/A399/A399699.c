/*
 * A399699  Decimal expansion of the probability that the cycle-following
 *          strategy succeeds in the 100 prisoners problem.
 * https://oeis.org/A399699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base, new
 * Formula:   a(n) = nth decimal digit after the point in
 *            21740752665556690246055199895649405434183 /
 *            69720375229712477164533808935312303556800
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation uses a tiny arbitrary‑precision bigint
 * sufficient for the 44‑digit numerator and denominator.
 * The bigint is stored in base 1e9 and never grows beyond 20 words.
 * Digits are computed on demand and cached.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define BIGINT_BASE 1000000000U
#define BIGINT_MAX_WORDS 20U

typedef struct {
    size_t len;                     /* number of used words */
    uint32_t d[BIGINT_MAX_WORDS];   /* little‑endian words */
} bigint_t;

/* initialize to zero */
static void bigint_zero(bigint_t *x) { x->len = 0; }

/* copy from y to x (no dynamic allocation) */
static void bigint_copy(const bigint_t *y, bigint_t *x) {
    x->len = y->len;
    for (size_t i = 0; i < y->len; ++i) x->d[i] = y->d[i];
}

/* compare a and b : return -1 if a<b, 0 if a==b, 1 if a>b */
static int bigint_cmp(const bigint_t *a, const bigint_t *b) {
    if (a->len < b->len) return -1;
    if (a->len > b->len) return 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] < b->d[i]) return -1;
        if (a->d[i] > b->d[i]) return 1;
    }
    return 0;
}

/* multiply by small 32‑bit integer m (0 <= m <= 10) */
static void bigint_mul_small(bigint_t *x, uint32_t m) {
    if (m == 0 || x->len == 0) { x->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry   = prod / BIGINT_BASE;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

/* subtract b from a (a >= b). */
static void bigint_sub(bigint_t *a, const bigint_t *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - borrow - (i < b->len ? b->d[i] : 0);
        if ((int64_t)sub < 0) {
            sub += BIGINT_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* parse decimal string into bigint.  s must consist of '0'..'9'. */
static void bigint_from_string(const char *s, bigint_t *x) {
    bigint_zero(x);
    for (; *s; ++s) {
        uint32_t digit = (uint32_t)(*s - '0');
        /* multiply current value by 10 */
        bigint_mul_small(x, 10);
        /* add digit */
        if (x->len == 0) x->d[x->len++] = digit;
        else {
            uint64_t sum = (uint64_t)x->d[0] + digit;
            x->d[0] = (uint32_t)(sum % BIGINT_BASE);
            uint64_t carry = sum / BIGINT_BASE;
            size_t i = 1;
            while (carry && i < x->len) {
                uint64_t ssum = (uint64_t)x->d[i] + carry;
                x->d[i] = (uint32_t)(ssum % BIGINT_BASE);
                carry = ssum / BIGINT_BASE;
                ++i;
            }
            if (carry) x->d[x->len++] = (uint32_t)carry;
        }
    }
}

/* compute prod = denom * small (0..9) and store in result */
static void bigint_mul_small_result(const bigint_t *denom, uint32_t small, bigint_t *result) {
    result->len = 0;
    if (small == 0 || denom->len == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < denom->len; ++i) {
        uint64_t prod = (uint64_t)denom->d[i] * small + carry;
        result->d[result->len++] = (uint32_t)(prod % BIGINT_BASE);
        carry = prod / BIGINT_BASE;
    }
    while (carry) {
        result->d[result->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

/* determine single decimal digit of current_remainder / denom and update remainder */
static uint32_t compute_next_digit_and_update(bigint_t *rem, const bigint_t *denom) {
    /* rem = rem * 10 */
    bigint_mul_small(rem, 10);

    /* binary search q in [0,9] : denom*q <= rem < denom*(q+1) */
    uint32_t low = 0, high = 9, mid, q = 0;
    while (low <= high) {
        mid = (low + high) / 2;
        bigint_t prod;
        bigint_mul_small_result(denom, mid, &prod);
        int cmp = bigint_cmp(&prod, rem);
        if (cmp <= 0) {
            q = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    /* rem = rem - denom*q */
    bigint_t prod;
    bigint_mul_small_result(denom, q, &prod);
    bigint_sub(rem, &prod);
    return q;
}

/* cache for computed decimal digits */
typedef struct {
    size_t capacity;
    size_t last;            /* index of last computed digit, (size_t)-1 if none */
    unsigned char *elems;
} digit_cache_t;

static void cache_init(digit_cache_t *c) {
    c->capacity = 128;
    c->last = (size_t)-1;
    c->elems = (unsigned char *)malloc(c->capacity);
}

static void cache_grow(digit_cache_t *c, size_t needed) {
    if (needed < c->capacity) return;
    size_t newcap = c->capacity * 2;
    while (newcap <= needed) newcap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(c->elems, newcap);
    if (!tmp) exit(1);                /* abort on allocation failure */
    c->elems = tmp;
    c->capacity = newcap;
}

/* global state */
static bool seq_initialized = false;
static bigint_t numerator;
static bigint_t denominator;
static bigint_t remainder;          /* current remainder */
static digit_cache_t digit_cache;

/* public function */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!seq_initialized) {
        /* constants */
        const char *num_str = "21740752665556690246055199895649405434183";
        const char *den_str = "69720375229712477164533808935312303556800";

        bigint_from_string(num_str, &numerator);
        bigint_from_string(den_str, &denominator);
        bigint_copy(&numerator, &remainder);
        cache_init(&digit_cache);
        seq_initialized = true;
    }

    size_t idx = (size_t)n;
    /* ensure cache big enough */
    cache_grow(&digit_cache, idx);

    /* compute missing digits */
    size_t start = (digit_cache.last == (size_t)-1) ? 0 : digit_cache.last + 1;
    for (size_t i = start; i <= idx; ++i) {
        unsigned char d = (unsigned char)compute_next_digit_and_update(&remainder, &denominator);
        digit_cache.elems[i] = d;
    }
    digit_cache.last = idx;

    return (seq_t)digit_cache.elems[idx];
}
