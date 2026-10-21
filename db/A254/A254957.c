/*
 * A254957  Triangular numbers n with digits d_1, d_2, ..., d_k such that
 *          d_1*(d_1+1)/2 + ... + d_k*(d_k+1)/2 is a triangular number.
 * https://oeis.org/A254957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq_array, floor_sqrt, free_seq_array, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A254957 – Triangular numbers n such that the sum of d_i*(d_i+1)/2 over its
    decimal digits is itself a triangular number.
    a(n) is the n‑th such triangular number (1‑based, a(1)=0).  */
#include <stddef.h>
#include "../utils/get_digits.h"          /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/floor_sqrt.h"          /* seq_t floor_sqrt(seq_t x); */
#include "../utils/alloc_seq_array.h"     /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"      /* void free_seq_array(seq_t *p); */

static seq_t *cache = NULL;      /* cached triangular numbers */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* capacity of cache array */
static idx_t  last_m   = 0;      /* last m processed (m such that T=m*(m+1)/2) */

/* Grow the cache to at least 'need' capacity. */
static void grow_cache(size_t need)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free_seq_array(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least n terms (n is 1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;      /* already have enough terms */

    if (!cache) {                            /* first time: create cache */
        cache = alloc_seq_array(1);
        cache_cap = 1;
        cache_len = 0;
        last_m = 0;
        /* T(0) = 0 is always a valid term. */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Pre‑computed d*(d+1)/2 for d=0..9. */
    static const unsigned char digit_contrib[10] =
        {0, 1, 3, 6, 10, 15, 21, 28, 36, 45};

    uint8_t buf[64];        /* buffer for digits of T(m) */
    size_t len;

    for (;;) {
        ++last_m;                                 /* next m */
        seq_t T = (seq_t)last_m * ((seq_t)last_m + 1) / 2;

        get_digits(T, buf, &len);

        seq_t sum = 0;
        for (size_t i = 0; i < len; ++i) {
            sum += (seq_t)digit_contrib[buf[i]];
        }

        /* Check whether sum is triangular: 8*sum+1 must be a perfect square */
        seq_t s = (seq_t)8 * sum + 1;
        seq_t r = floor_sqrt(s);
        if (r * r == s) {
            if (cache_len == cache_cap) grow_cache(cache_len + 1);
            cache[cache_len++] = T;
            if (cache_len >= (size_t)n) return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds – safety */
    ensure_cache(n);                       /* now cache_len >= n */
    return cache[n - 1];
}
