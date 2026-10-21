/*
 * A279144  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 137", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A279144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = - 2*a(n-1) + 8*a(n-3) + 16*a(n-4) + a(n-8) + 2*a(n-9) -
 *            8*a(n-11) - 16*a(n-12) for n > 15 - Initial values:
 *            a(0)=1,a(1)=0,a(2)=5,a(3)=0,a(4)=21,a(5)=0,a(6)=85,a(7)=0,
 *            a(8)=373,a(9)=160,a(10)=1045,a(11)=896,a(12)=5493,a(13)=160,
 *            a(14)=21525,a(15)=896
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements the sequence using the above
   linear recurrence, storing intermediate big integers in a
   simple limb‑based representation so that it works for both
   fixed‑width and arbitrary‑precision builds. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

#define BASE 1000000000U      /* base 1e9 for limb representation */

typedef struct {
    uint32_t *limb;   /* little‑endian limbs, each < BASE */
    size_t len;       /* number of limbs, 0 means zero */
} bigint_t;

/* --------------------------------------------------------------------- */
/* Basic bigint operations (only +, - and multiply by small constants) */

static void bigint_init_zero(bigint_t *a) {
    a->limb = NULL;
    a->len  = 0;
}

static void bigint_from_uint64(bigint_t *a, uint64_t v) {
    bigint_init_zero(a);
    if (v == 0) return;
    while (v) {
        a->limb = (uint32_t*)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        a->limb[a->len++] = (uint32_t)(v % BASE);
        v /= BASE;
    }
}

static void bigint_copy(const bigint_t *src, bigint_t *dst) {
    bigint_init_zero(dst);
    if (src->len == 0) return;
    dst->limb = (uint32_t*)malloc(src->len * sizeof(uint32_t));
    memcpy(dst->limb, src->limb, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void bigint_free(bigint_t *a) {
    if (a->limb) {
        free(a->limb);
        a->limb = NULL;
    }
    a->len = 0;
}

static void bigint_add(bigint_t *a, const bigint_t *b) {
    size_t maxlen = (a->len > b->len) ? a->len : b->len;
    a->limb = (uint32_t*)realloc(a->limb, (maxlen + 1) * sizeof(uint32_t));
    uint64_t carry = 0, sum;
    size_t i;
    for (i = 0; i < maxlen; ++i) {
        sum = carry;
        if (i < a->len) sum += a->limb[i];
        if (i < b->len) sum += b->limb[i];
        a->limb[i] = (uint32_t)(sum % BASE);
        carry     = sum / BASE;
    }
    if (carry) {
        a->limb[i++] = (uint32_t)carry;
    }
    a->len = i;
    while (a->len && a->limb[a->len - 1] == 0) a->len--;
}

static void bigint_sub(bigint_t *a, const bigint_t *b) {
    /* assumes a >= b */
    uint64_t borrow = 0, sub;
    for (size_t i = 0; i < a->len; ++i) {
        sub = (uint64_t)a->limb[i] - borrow;
        if (i < b->len) sub -= b->limb[i];
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->limb[i] = (uint32_t)sub;
    }
    while (a->len && a->limb[a->len - 1] == 0) a->len--;
}

static void bigint_mul_small(bigint_t *a, uint32_t m) {
    if (a->len == 0 || m == 1) return;
    uint64_t carry = 0, prod;
    for (size_t i = 0; i < a->len; ++i) {
        prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BASE);
        carry     = prod / BASE;
    }
    while (carry) {
        a->limb = (uint32_t*)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        a->limb[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void add_scaled(const bigint_t *src, uint32_t factor, bigint_t *dest) {
    if (factor == 0) return;
    bigint_t tmp;
    bigint_copy(src, &tmp);
    bigint_mul_small(&tmp, factor);
    bigint_add(dest, &tmp);
    bigint_free(&tmp);
}

static char *bigint_to_str(const bigint_t *a) {
    if (a->len == 0) {
        char *s = (char*)malloc(2);
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    size_t est = a->len * 9 + 1;
    char *s = (char*)malloc(est + 1);
    size_t pos = 0;
    int ms = (int)a->len - 1;
    pos += snprintf(s + pos, est + 1 - pos, "%u", a->limb[ms]);
    for (int i = ms - 1; i >= 0; --i) {
        pos += snprintf(s + pos, est + 1 - pos, "%09u", a->limb[i]);
    }
    s[pos] = '\0';
    return s;
}

/* --------------------------------------------------------------------- */
/* Cache for computed terms */

static bigint_t *cache = NULL;
static size_t cache_size = 0;       /* capacity of cache */
static size_t first_uncomputed = 0; /* index of first term not yet computed */

static void init_cache(void) {
    if (cache != NULL) return;
    cache_size = 16;
    cache = (bigint_t*)malloc(cache_size * sizeof(bigint_t));
    for (size_t i = 0; i < cache_size; ++i) bigint_init_zero(&cache[i]);

    /* base values 0..15 */
    bigint_from_uint64(&cache[0], 1);
    bigint_from_uint64(&cache[1], 0);
    bigint_from_uint64(&cache[2], 5);
    bigint_from_uint64(&cache[3], 0);
    bigint_from_uint64(&cache[4], 21);
    bigint_from_uint64(&cache[5], 0);
    bigint_from_uint64(&cache[6], 85);
    bigint_from_uint64(&cache[7], 0);
    bigint_from_uint64(&cache[8], 373);
    bigint_from_uint64(&cache[9], 160);
    bigint_from_uint64(&cache[10], 1045);
    bigint_from_uint64(&cache[11], 896);
    bigint_from_uint64(&cache[12], 5493);
    bigint_from_uint64(&cache[13], 160);
    bigint_from_uint64(&cache[14], 21525);
    bigint_from_uint64(&cache[15], 896);
    first_uncomputed = 16;
}

/* Ensure cache holds terms up to n */
static void ensure_cache(idx_t n) {
    init_cache();
    if ((size_t)n < first_uncomputed) return;

    /* grow cache if required */
    size_t needed = (size_t)n + 1;
    if (cache_size < needed) {
        size_t new_size = cache_size * 2;
        if (new_size < needed) new_size = needed;
        bigint_t *new_cache = (bigint_t*)realloc(cache, new_size * sizeof(bigint_t));
        cache = new_cache;
        for (size_t i = cache_size; i < new_size; ++i) bigint_init_zero(&cache[i]);
        cache_size = new_size;
    }

    for (size_t k = first_uncomputed; k <= (size_t)n; ++k) {
        bigint_t pos, neg;
        bigint_init_zero(&pos);
        bigint_init_zero(&neg);

        /* positive part: a[k-8], 2*a[k-9], 8*a[k-3], 16*a[k-4] */
        add_scaled(&cache[k - 8], 1, &pos);
        add_scaled(&cache[k - 9], 2, &pos);
        add_scaled(&cache[k - 3], 8, &pos);
        add_scaled(&cache[k - 4], 16, &pos);

        /* negative part: 2*a[k-1], 8*a[k-11], 16*a[k-12] */
        add_scaled(&cache[k - 1], 2, &neg);
        add_scaled(&cache[k - 11], 8, &neg);
        add_scaled(&cache[k - 12], 16, &neg);

        /* a[k] = pos - neg  (pos >= neg for all k) */
        bigint_sub(&pos, &neg);

        /* move result into cache */
        cache[k] = pos;         /* copy pointer and len */
        /* pos now owned by cache; protect from freeing */
        pos.limb = NULL;
        pos.len  = 0;

        /* free neg llimbs */
        bigint_free(&neg);
    }

    first_uncomputed = (size_t)n + 1;
}

/* --------------------------------------------------------------------- */
/* Public API */

seq_t a(idx_t n) {
    ensure_cache(n);
    /* convert bigint to string then parse to seq_t */
    char *s = bigint_to_str(&cache[(size_t)n]);
    seq_t result = seq_from_str(s);
    free(s);
    return result;
}
