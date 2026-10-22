/*
 * A319317  Numbers k such that A090616(k) > A054861(k).
 * https://oeis.org/A319317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that * floor(v2(k!)/2) >
 *            v3(k!) * (i.e., the highest power of 4 dividing k! exceeds the
 *            highest power * of 3 dividing k!)
 * Range:     u8 n <= 108, u16 n <= 5071, u32 n <= 5071, u64 n <= 5071,
 *            u128 n <= 5071, big any n
 * Helpers:   exp_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/exp_prime.h"

/* Append a value to a dynamic array, reallocating if necessary.  The
 * array is managed with a pointer, its current length, and capacity. */
static void append_term(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap)
    {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *new_cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return;           /* allocation failure – stop growing */
        *cache = new_cache;
        *cap   = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* Return the n-th term of the sequence (a(1)=8).  Calls may appear in
 * arbitrary order; the sequence is produced lazily and cached. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t last_checked = 0;  /* last k examined */

    if (n == 0)
        return 0;          /* Undefined for n < 1 – arbitrary value */

    while (cache_len < (size_t)n)
    {
        ++last_checked;              /* candidate k */
        seq_t e2 = exp_prime(last_checked, 2);
        seq_t e3 = exp_prime(last_checked, 3);
        if ((e2 >> 1) > e3)          /* floor(v2/2) > v3 */
        {
            append_term(&cache, &cache_len, &cache_cap, (seq_t)last_checked);
        }
    }

    return cache[n - 1];
}
