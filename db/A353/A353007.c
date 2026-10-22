/*
 * A353007  Nonnegative integers in which any odd digit, if present, occurs an
 *          even number of times, and any even digit, if present, occurs an
 *          odd number of times.
 * https://oeis.org/A353007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth nonnegative integer in which every odd digit, if
 *            present, occurs an even number of times, and every even digit,
 *            if present, occurs an odd number of times
 * Range:     u8 n <= 46, u16 n <= 4023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates terms on demand, storing those found
   so far in a static cache so that subsequent calls are cheap. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 0;

/* Grow the cache to accommodate at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Check whether a nonnegative integer has the required digit parity. */
static bool valid_candidate(unsigned long long x)
{
    unsigned int parity = 0; /* parity of counts (bit i = 1 if odd count of digit i) */
    unsigned int seen   = 0; /* digits that have appeared at least once */
    if (x == 0) {
        parity = 1u << 0;
        seen   = 1u << 0;
    } else {
        while (x > 0) {
            int d = x % 10;
            parity ^= 1u << d;
            seen   |= 1u << d;
            x /= 10;
        }
    }
    /* Odd digits must have even count: parity bit must be 0 */
    const unsigned int oddMask = (1u << 1) | (1u << 3) |
                                 (1u << 5) | (1u << 7) |
                                 (1u << 9);
    if (parity & oddMask)
        return false;
    /* For even digits, if present parity must be 1 (odd count) */
    const unsigned int evenMask = (1u << 0) | (1u << 2) |
                                  (1u << 4) | (1u << 6) |
                                  (1u << 8);
    unsigned int presentEven = seen & evenMask;
    /* For each even digit present, the corresponding parity bit
       must be set; otherwise (presentEven & ~parity) is non-zero. */
    if ((presentEven & ~parity) != 0)
        return false;
    return true;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    if (n == 0)
        return (seq_t)0; /* undefined, but guard */
    /* Generate terms until we have at least n. */
    while (cache_len < (size_t)n) {
        if (valid_candidate(next_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len] = (seq_t)next_candidate;
            cache_len++;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
