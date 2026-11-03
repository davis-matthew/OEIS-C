/*
 * A341002  Numbers whose sum of even digits and sum of odd digits differ by
 *          1.
 * https://oeis.org/A341002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   numbers whose sum of even digits and sum of odd digits differ by
 *            1 (computed via a brute‑force search with memoisation)
 * Range:     u8 n <= 32, u16 n <= 3901, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Validated: standard, pass, 75 terms through n = 4903 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether x has |sum_even - sum_odd| == 1 */
static bool diff_one(seq_t x)
{
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    while (x > 0)
    {
        seq_t d = x % 10;
        if ((d & 1) == 0)
            sum_even += d;
        else
            sum_odd += d;
        x /= 10;
    }
    seq_t diff = sum_even > sum_odd ? sum_even - sum_odd : sum_odd - sum_even;
    return diff == 1;
}

/* Grow the cache if needed (doubling strategy). */
static void ensure_cap(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need)
        new_cap <<= 1;
    *ptr = mem_resize(seq_t, *ptr, new_cap);
    *cap  = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static seq_t current     = 1;   /* next integer to test */

    if (n == 0)
        return 0;   /* sequence is defined for n >= 1 */

    /* Compute terms up to n if not already */
    while (cache_len < n)
    {
        if (diff_one(current))
        {
            ensure_cap(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = current;
        }
        current++;
    }
    return cache[n - 1];
}
