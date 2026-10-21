/*
 * A247565  a(n) = 5*a(n-1) - 10*a(n-2) + 8*a(n-3) with a(0) = 2, a(1) = a(2)
 *          = 3.
 * https://oeis.org/A247565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 31, i64 n <= 65, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 10*a(n-2) + 8*a(n-3)
   with initial values a(0)=2, a(1)=3, a(2)=3. */

#include <stddef.h>

#define MAX_N 12000   /* safe upper bound (≈10 000) */

static seq_t cache[MAX_N+1];
static size_t cached_len = 0;

/* Compute values up to n and store them in cache. */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n;
    if (target < cached_len) return;

    for (size_t i = cached_len; i <= target; ++i)
    {
        if (i < 3) {
            if (i == 0) cache[0] = (seq_t)2;
            else if (i == 1) cache[1] = (seq_t)3;
            else          cache[2] = (seq_t)3;
        } else {
            cache[i] = ((seq_t)5 * cache[i-1]) 
                     - ((seq_t)10 * cache[i-2]) 
                     + ((seq_t)8 * cache[i-3]);
        }
    }
    cached_len = target + 1;
}

/* External function exposed to callers. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
