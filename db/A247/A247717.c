/*
 * A247717  Numbers n such that A033493(n)/A008908(n) is an integer.
 * https://oeis.org/A247717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   collatz_avg_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A247717 (offset 1). */
/* A number n is accepted iff the average of its Collatz 3x+1 iteration
 * is an integer.  The routine generates terms on demand and memoises
 * them in a static array. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/collatz_avg_integer.h"

static seq_t *cache = NULL;          /* array of accepted numbers */
static size_t cache_cap = 0;         /* current capacity */
static size_t cache_len = 0;         /* number of stored terms */
static idx_t  next_candidate = 1;    /* next number to test */

/* Ensure the static cache has room for at least 'needed' terms. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/*  a(idx_t n)  –  Return the n‑th term of sequence A247717 (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1: n should be >=1; return 0 for safety */
        return 0;

    ensure_cache_capacity((size_t)n);

    while (cache_len < (size_t)n) {
        if (collatz_avg_integer(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
