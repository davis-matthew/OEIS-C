/*
 * A347268  a(n) is the first term of the n-th 3x+1 sequence that shares
 *          infinitely many 1's with the 3x+1 sequence that starts at 2.
 * https://oeis.org/A347268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   total_stop_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th natural number m whose Collatz total
 * stopping time is congruent to 1 (mod 3).  The sequence is generated
 * by scanning integers upwards and selecting those with that property. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/total_stop_mod3.h"

/* ----------------- helper: total stopping time modulo 3 ----------------- */

/* ----------------- dynamic cache of valid terms ----------------- */
static seq_t *cache = NULL;      /* array of terms */
static idx_t  cache_len = 0;     /* number of terms stored   */
static idx_t  cache_cap = 0;     /* allocated capacity       */
static seq_t  last_checked = (seq_t)1;  /* last m examined       */

/* Ensure that the cache can hold at least 'needed' terms.  Caller
 * guarantees that 'needed' > 'cache_cap' if a reallocation is
 * necessary. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_arr)                    /* allocation failure – leave cache */
        return;
    cache = new_arr;
    cache_cap = new_cap;
}

/* ----------------- public sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n==0 */

    /* Generate terms until we have at least n cached. */
    while (cache_len < n) {
        seq_t m = ++last_checked;           /* next candidate number */

        if (total_stop_mod3(m) == 1) {       /* condition for the sequence */
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = m;
        }
    }
    return cache[n - 1];
}
