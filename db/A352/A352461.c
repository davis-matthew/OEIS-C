/*
 * A352461  Numbers k whose decimal expansion starts with the sum of digits of
 *          k.
 * https://oeis.org/A352461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A352461 - Numbers k whose decimal expansion starts with the sum of digits of k.
   The function generates the sequence by brute‑force scanning, storing terms in a
   static cache and expanding it as needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the sum of the decimal digits of x. */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    const seq_t ten = (seq_t)10;
    while (x > 0) {
        sum += x % ten;
        x /= ten;
    }
    return sum;
}

/* Helper: count decimal digits of x (x > 0). */
static unsigned int digit_count(seq_t x)
{
    unsigned int cnt = 0;
    const seq_t ten = (seq_t)10;
    while (x > 0) {
        ++cnt;
        x /= ten;
    }
    return cnt;
}

/* Static cache for computed terms. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
/* The last integer that has been tested. */
static seq_t last_checked = 0;

/* Ensure the cache is large enough. */
static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* In contest code one may abort on failure; we ignore here. */
        cache = new_arr;
        cache_cap = new_cap;
    }
}

/* Test whether k satisfies the property. */
static bool is_term(seq_t k)
{
    seq_t sum    = digit_sum(k);
    unsigned int d_sum = digit_count(sum);
    unsigned int d_k   = digit_count(k);

    /* Compute 10^(d_k - d_sum). */
    const seq_t ten = (seq_t)10;
    seq_t pow10 = 1;
    unsigned int exp = d_k - d_sum;
    for (unsigned int i = 0; i < exp; ++i) {
        pow10 *= ten;
    }

    seq_t prefix = k / pow10;
    return prefix == sum;
}

/* Return the nth term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but guard */

    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        last_checked = 0;
    }

    /* Expand cache up to n terms if necessary. */
    while (cache_size < (size_t)n) {
        seq_t k = last_checked + 1;
        if (is_term(k)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = k;
        }
        last_checked = k;
    }

    return cache[n - 1];
}
