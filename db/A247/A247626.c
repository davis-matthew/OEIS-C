/*
 * A247626  Knife numbers (third version): n=concat(a,b) is divisible by a+b;
 *          a>0, b may have leading zeros.
 * https://oeis.org/A247626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer >=10 that can be split into a>0 and b
 *            (leading zeros allowed) with * concat(a,b) divisible by (a+b).
 *            The function generates successive terms lazily * and caches them
 *            to support arbitrary order of calls
 * Range:     u8 n <= 57, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_knife_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_knife_number.h"

/* static cache for already computed terms */
static seq_t *knife_cache = NULL;
static size_t knife_cache_size = 0;
static size_t knife_cache_cap  = 0;
static seq_t knife_last_checked = 9;   /* last integer examined (start just below first term) */

/* appends a new term to the cache, growing the buffer if needed */
static void knife_cache_append(seq_t val)
{
    if (knife_cache_size == knife_cache_cap) {
        size_t new_cap = knife_cache_cap ? knife_cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(knife_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out‑of‑memory guard */
        knife_cache = tmp;
        knife_cache_cap = new_cap;
    }
    knife_cache[knife_cache_size++] = val;
}

/* public function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (knife_cache_size < (size_t)n) {             /* need to generate more terms */
        for (;;) {
            ++knife_last_checked;                   /* next candidate number */
            if (is_knife_number(knife_last_checked))
                knife_cache_append(knife_last_checked);
            if (knife_cache_size >= (size_t)n) break;
        }
    }
    return knife_cache[n - 1];                        /* 1‑based indexing */
}
