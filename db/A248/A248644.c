/*
 * A248644  Evil numbers (A001969) remaining evil if any digit is deleted
 *          (zeros allowed).
 * https://oeis.org/A248644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_digits_seq, is_even_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are evil (even number of 1's in binary) and remain
 * evil after deleting any single decimal digit.  Implemented by brute‑force
 * search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits_seq.h"
#include "../utils/is_even_ones.h"

/* Check whether deleting any single decimal digit from x yields an evil number. */
static bool good_candidate(seq_t x)
{
    static uint8_t buf[1024];               /* large enough for any expected width */
    size_t len;
    get_digits_seq(x, buf, &len);

    for (size_t i = 0; i < len; ++i)
    {
        seq_t y = (seq_t)0;
        for (size_t j = 0; j < len; ++j)
        {
            if (j == i)
                continue;
            y = y * (seq_t)10 + (seq_t)buf[j];
        }
        if (!is_even_ones(y))
            return false;
    }
    return true;
}

/* Cache for already found sequence terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 0;

/* Grow the cache until it contains at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;

    if (cache_cap == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    while (cache_len < (size_t)need)
    {
        seq_t x = (seq_t)next_candidate++;
        if (!is_even_ones(x))
            continue;
        if (!good_candidate(x))
            continue;

        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = x;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* out‑of‑offset access – not used in tests */
    ensure_cache(n);
    return cache[n - 1];
}
