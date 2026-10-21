/*
 * A285767  Cyclops octagonal numbers: a(n) = n*(3*n-2) with one "zero" digit
 *          in the middle.
 * https://oeis.org/A285767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A285767: cyclops octagonal numbers.
   a(n) is the nth octagonal number n*(3*n-2) whose decimal representation
   has an odd number of digits with exactly one zero in the middle.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_len.h"

/* Check whether the octagonal number satisfies the cyclops property. */
static bool is_cyclops_octagonal(seq_t x)
{
    int d = digit_len(x);
    if ((d & 1) == 0)          /* even number of digits → reject */
        return false;
    int mid = d / 2;           /* position of the middle digit (0‑based) */
    seq_t y = x;
    for (int pos = 0; pos < d; ++pos) {
        int digit = (int)(y % 10);
        if (pos == mid) {
            if (digit != 0)     /* middle must be zero */
                return false;
        } else {
            if (digit == 0)     /* other digits must be non‑zero */
                return false;
        }
        y /= 10;
    }
    return true;
}

/* Cache for the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_i = 0;      /* index for the next octagonal number to test */

/* Ensure capacity in the cache. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* out of bounds; definition unspecified */

    while (cache_size < (size_t)n) {
        seq_t term = (seq_t)next_i * (((seq_t)next_i) * 3 - 2);
        if (is_cyclops_octagonal(term)) {
            if (cache_size == cache_cap)
                grow_cache();
            cache[cache_size++] = term;
        }
        ++next_i;
    }
    return cache[(size_t)n - 1];
}
