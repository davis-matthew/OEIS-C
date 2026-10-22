/*
 * A350444  a(n) is the smallest number that has not appeared yet in the
 *          sequence and has only one digit in common with a(n-1).
 * https://oeis.org/A350444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A350444: a(n) = smallest unused positive integer sharing exactly
   one digit (appearing exactly once in both numbers) with a(n-1),
   a(1)=1. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* cached sequence */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure cache can hold at least `need` terms. */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return true if x is already in the cache. */
static bool used(seq_t x)
{
    for (idx_t i = 0; i < cache_len; ++i)
        if (cache[i] == x) return true;
    return false;
}

/* Fill `cnt` with the count of each decimal digit in `x`. */
static void digit_counts(seq_t x, unsigned char cnt[10])
{
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    if (x == (seq_t)0) {
        cnt[0] = 1;
        return;
    }
    seq_t t = x;
    while (t != (seq_t)0) {
        unsigned int d = (unsigned int)(t % (seq_t)10);
        ++cnt[d];
        t /= (seq_t)10;
    }
}

/* Two numbers share exactly one digit, and that digit appears exactly
   once in each number. */
static bool one_shared_digit_unique(seq_t a, seq_t b)
{
    unsigned char ca[10], cb[10];
    digit_counts(a, ca);
    digit_counts(b, cb);
    int common = 0;
    unsigned int digit = 0;
    for (unsigned int d = 0; d < 10; ++d) {
        if (ca[d] && cb[d]) {
            ++common;
            digit = d;
            if (common > 1) return false;
        }
    }
    return (common == 1 && ca[digit] == 1 && cb[digit] == 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return 0 safely */
    if (cache_len < n) {
        grow_cache(n);
        /* compute missing terms up to n */
        for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
            seq_t val;
            if (idx == 1) {
                val = (seq_t)1;
            } else {
                seq_t prev = cache[cache_len - 1];
                seq_t cand = (seq_t)1;
                for (;;) {
                    if (!used(cand) && one_shared_digit_unique(prev, cand)) {
                        val = cand;
                        break;
                    }
                    ++cand;
                }
            }
            cache[cache_len++] = val;
        }
    }
    return cache[n - 1];
}
