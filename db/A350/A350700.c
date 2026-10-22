/*
 * A350700  a(n) is the number of 1's minus the number of 0's in A004685(n).
 * https://oeis.org/A350700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = 2*popcount(Fibonacci(n)) - bitlen(Fibonacci(n)) for n >=
 *            1 a(0) = -1 (where Fibonacci(1) = 1, Fibonacci(2) = 1,
 *            Fibonacci(3) = 2, ...) This implementation computes Fibonacci
 *            numbers using a 64‑bit word big‑integer representation,
 *            calculates their popcount and bit length, and memoises the s...
 * Range:     i8 n <= 2848, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* internal big–integer representation */
typedef struct {
    uint64_t *d;   /* little‑endian words */
    size_t   len;  /* number of used words */
    size_t   cap;  /* allocated word count */
} BigInt;

static void BigInt_init(BigInt *x)
{
    x->d   = NULL;
    x->len = 0;
    x->cap = 0;
}

static void BigInt_resize(BigInt *x, size_t newlen)
{
    if (newlen > x->cap) {
        size_t newcap = newlen * 2 + 1;
        uint64_t *tmp = (uint64_t *)realloc(x->d, newcap * sizeof(uint64_t));
        x->d   = tmp;
        x->cap = newcap;
    }
    x->len = newlen;
}

static void BigInt_assign(BigInt *dest, const BigInt *src)
{
    BigInt_resize(dest, src->len);
    if (src->len > 0)
        memcpy(dest->d, src->d, src->len * sizeof(uint64_t));
}

static void BigInt_add(const BigInt *a, const BigInt *b, BigInt *res)
{
    size_t maxlen = (a->len > b->len) ? a->len : b->len;
    BigInt_resize(res, maxlen + 1);
    __uint128_t carry = 0;
    size_t i;
    for (i = 0; i < maxlen; ++i) {
        __uint128_t va = (i < a->len) ? a->d[i] : 0;
        __uint128_t vb = (i < b->len) ? b->d[i] : 0;
        __uint128_t sum = va + vb + carry;
        res->d[i] = (uint64_t)sum;
        carry     = sum >> 64;
    }
    if (carry) {
        res->d[i] = (uint64_t)carry;
        i++;
    }
    res->len = i;
}

static int BigInt_popcount(const BigInt *x)
{
    int pc = 0;
    for (size_t i = 0; i < x->len; ++i)
        pc += __builtin_popcountll(x->d[i]);
    return pc;
}

static size_t BigInt_bitlen(const BigInt *x)
{
    if (x->len == 0) return 0;
    uint64_t ms = x->d[x->len - 1];
    int leading = __builtin_clzll(ms);
    return (x->len - 1) * 64 + (64 - leading);
}

/* requested function */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool       initialized = false;
    static seq_t     *cache        = NULL;    /* holds a[0..current_max] */
    static idx_t      cache_len    = 0;      /* number of stored terms */
    static idx_t      current_k    = 0;      /* index of last Fibonacci used */
    static BigInt     f_prev, f_curr, f_next;

    /* initialisation on first call */
    if (!initialized) {
        /* cache[0] = -1 (a(0)) */
        cache      = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0]   = (seq_t)-1;
        cache[1]   = 0;      /* placeholder for a(1) */
        cache_len  = 2;

        /* initialise big‑integers for Fibonacci */
        BigInt_init(&f_prev);           /* F0 = 0 */
        BigInt_init(&f_curr);           /* F1 = 1 */
        BigInt_resize(&f_curr, 1);
        f_curr.d[0] = 1;
        BigInt_init(&f_next);

        /* compute a(1) from F1 */
        int ones   = BigInt_popcount(&f_curr);
        size_t bl  = BigInt_bitlen(&f_curr);
        cache[1]   = (seq_t)(2 * ones - (seq_t)bl);

        current_k = 1;  /* last computed Fibonacci index */
        initialized = true;
    }

    /* requested term zero */
    if (n == 0)
        return cache[0];

    /* if already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache to hold up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* malloc failure – return zero, though this is unlikely in tests */
        return (seq_t)0;
    }
    cache = new_cache;
    cache_len = n + 1;

    /* compute successive Fibonacci numbers and sequence terms */
    while (current_k < n) {
        /* f_next = f_prev + f_curr */
        BigInt_add(&f_prev, &f_curr, &f_next);

        /* shift: f_prev = f_curr, f_curr = f_next */
        BigInt_assign(&f_prev, &f_curr);
        BigInt_assign(&f_curr, &f_next);

        ++current_k;  /* now f_curr represents F_current_k */

        /* compute a(current_k) = 2*popcount - bitlen */
        int ones  = BigInt_popcount(&f_curr);
        size_t bl = BigInt_bitlen(&f_curr);
        cache[current_k] = (seq_t)(2 * ones - (seq_t)bl);
    }

    return cache[n];
}
