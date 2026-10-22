/*
 * A342445  Numbers that are divisible by their nonzero digits but are not
 *          divisible by the product of their nonzero digits.
 * https://oeis.org/A342445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer that is divisible by every
 *            non‑zero digit of the integer but is NOT divisible by the
 *            product of its non‑zero digits. The sequence is searched by
 *            brute force with a memoizing cache. The code works for any seq_t
 *            (C integer type or arbitrary‑precision class) and any...
 * Range:     u8 n <= 21, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc for any seq_t */

/* Return true if x satisfies the property. */
static bool is_term(seq_t x)
{
    seq_t prod = (seq_t)1;
    seq_t tmp  = x;
    bool has_nonzero = false;

    while (tmp != (seq_t)0)
    {
        uint8_t digit = (uint8_t)(tmp % (seq_t)10);
        if (digit != 0)
        {
            has_nonzero = true;
            if ((x % (seq_t)digit) != (seq_t)0) return false;
            prod *= (seq_t)digit;
        }
        tmp /= (seq_t)10;
    }
    if (!has_nonzero) return false;
    return (x % prod) != (seq_t)0;
}

/* Cached terms of the sequence. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = (seq_t)0;

/* Grow the cache array to at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;

    if (cache == NULL)          /* first allocation */
        cache = seq_realloc(NULL, 0, new_cap);
    else
        cache = seq_realloc(cache, cache_cap, new_cap);

    cache_cap = new_cap;
}

/* Main function definition required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Ensure we have computed at least n terms. */
    while (cache_size < (size_t)n)
    {
        last_checked = last_checked + (seq_t)1;
        if (is_term(last_checked))
        {
            grow_cache(cache_size + 1);
            cache[cache_size++] = last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
