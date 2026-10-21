/*
 * A248899  Numbers that are palindromic in bases 10 and 19.
 * https://oeis.org/A248899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 27, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   grow_seq, is_palindrome, make_palindrome_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate decimal palindromes in increasing order
   and keep those that are also palindromes in base 19. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/grow_seq.h"
#include "../utils/make_palindrome_u64.h"

static seq_t *cache = NULL;   /* cached values */
static size_t cache_cap = 0;  /* capacity of cache */
static size_t cache_size = 0; /* number of filled entries */

/* Generation state (valid only after first call to ensure_cache) */
static int cur_len = 1;       /* current palindrome total length */
static idx_t cur_half_start = 0; /* first half value for current length */
static idx_t cur_half_end   = 9;  /* last half value for current length */
static idx_t cur_half = 0;   /* current half being processed */
static bool initialized = false;

/* Integer power of ten, safe for lengths needed in this problem */
static inline idx_t pow10_u64(int exp)
{
    idx_t val = 1;
    for (int i = 0; i < exp; ++i) val *= 10;
    return val;
}

/* Ensure the cache contains at least 'n' terms */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n)
        grow_seq(&cache, &cache_cap, n);

    if (!initialized) {
        cur_len = 1;
        cur_half_start = 0;
        cur_half_end   = 9;
        cur_half = cur_half_start;
        initialized = true;
    }

    while (cache_size < (size_t)n) {
        seq_t val = make_palindrome_u64(cur_half, cur_len);
        if (is_palindrome(val, 19))
            cache[cache_size++] = val;

        /* move to next half */
        ++cur_half;
        if (cur_half > cur_half_end) {
            /* advance to next length */
            ++cur_len;
            int half_len = (cur_len + 1) / 2;
            cur_half_start = (cur_len == 1) ? 0 : pow10_u64(half_len - 1);
            cur_half_end   = pow10_u64(half_len) - 1;
            cur_half = cur_half_start;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* offset is 1 – behaviour below is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
