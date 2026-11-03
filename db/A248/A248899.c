/*
 * A248899  Numbers that are palindromic in bases 10 and 19.
 * https://oeis.org/A248899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 27, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate decimal palindromes in increasing order
   and keep those that are also palindromes in base 19. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Build a decimal palindrome from its first half 'half' and total
   length 'len'.  For an odd length the middle digit is not repeated. */
static seq_t make_palindrome_u64(idx_t half, int len)
{
    /* Extract digits of half into an array */
    int half_len = (len + 1) / 2;
    idx_t digits[20];        /* enough for any 64‑bit half */
    idx_t tmp = half;
    for (int i = half_len - 1; i >= 0; --i) {
        digits[i] = tmp % 10;
        tmp /= 10;
    }

    /* Construct the palindrome value */
    idx_t res = 0;
    for (int i = 0; i < half_len; ++i)             /* first half */
        res = res * 10 + digits[i];

    int start = len & 1;                           /* skip middle digit if odd */
    for (int i = half_len - 1 - start; i >= 0; --i)/* second half */
        res = res * 10 + digits[i];

    return (seq_t)res;
}

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
        if (is_palindrome_base(val, 19))
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
