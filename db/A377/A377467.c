/*
 * A377467  Number of perfect-powers x in the range 2^n < x < 2^(n+1).
 * https://oeis.org/A377467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of perfect powers x with 2^n < x < 2^(n+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 28, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   bitlen_seq_t, cmp_seq_t, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"      /* pow2(idx_t) -> seq_t */
#include "../utils/pow_seq.h"   /* pow_seq(seq_t base, int exp) -> seq_t */
#include "../utils/bitlen_seq_t.h" /* bitlen_seq_t(seq_t) -> idx_t */
#include "../utils/cmp_seq_t.h"   /* cmp_seq_t(const void*, const void*) */

/* Simple cache for memoised values of F(x) */
struct cache_entry {
    seq_t key;
    seq_t val;
};
static struct cache_entry *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static int cache_lookup(seq_t key, seq_t *out)
{
    for (size_t i = 0; i < cache_len; ++i) {
        if (cmp_seq_t(&key, &cache[i].key) == 0) {
            *out = cache[i].val;
            return 1;
        }
    }
    return 0;
}

static void cache_insert(seq_t key, seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        struct cache_entry *tmp = (struct cache_entry *)realloc(cache,
                           new_cap * sizeof(struct cache_entry));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len].key = key;
    cache[cache_len].val = val;
    ++cache_len;
}

/* Fast e‑th integer root of x (return floor(x^(1/e))) */
static seq_t floor_root(seq_t x, idx_t e)
{
    if (e <= 1) return x;

    /* Initial guess: 2^(floor(bitlen(x)/e)) */
    idx_t bl = bitlen_seq_t(x);
    idx_t exp_guess = bl / e;
    seq_t a = pow2(exp_guess);          /* lower bound candidate */

    /* Adjust upward as long as (a+1)^e <= x */
    seq_t ap1 = a + 1;
    seq_t ap1_pow = pow_seq(ap1, (int)e);
    while (cmp_seq_t(&ap1_pow, &x) <= 0) {
        a = ap1;
        ap1 = a + 1;
        ap1_pow = pow_seq(ap1, (int)e);
    }

    /* Adjust downward if a^e > x */
    seq_t a_pow = pow_seq(a, (int)e);
    while (cmp_seq_t(&a_pow, &x) > 0) {
        a = a - 1;
        a_pow = pow_seq(a, (int)e);
    }
    return a;
}

/* Recursive count of distinct perfect powers <= x (including powers of 2) */
static seq_t F(seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)0;

    seq_t cached;
    if (cache_lookup(x, &cached)) return cached;

    idx_t emax = bitlen_seq_t(x) - 1;
    seq_t sum = (seq_t)0;

    for (idx_t e = 2; e <= emax; ++e) {
        seq_t b = floor_root(x, e);      /* base up to which a^e <= x */
        if (b <= (seq_t)1) continue;     /* no valid base */

        seq_t sub = b - (seq_t)1 - F(b);
        sum += sub;
    }

    cache_insert(x, sum);
    return sum;
}

/* Public sequence function: a(n) = F(2^(n+1)-1) - F(2^n) */
seq_t a(idx_t n)
{
    seq_t lowPow  = pow2(n);                     /* 2^n               */
    seq_t highPow = pow2(n + (idx_t)1);           /* 2^(n+1)            */
    seq_t upper   = highPow - (seq_t)1;          /* 2^(n+1)-1          */

    seq_t cntUpper = F(upper);
    seq_t cntLower = F(lowPow);

    return cntUpper - cntLower;
}
