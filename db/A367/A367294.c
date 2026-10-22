/*
 * A367294  Numbers k such that 4 is the first digit of 2^k.
 * https://oeis.org/A367294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the first decimal digit of
 *            2^k is 4. It is computed by iterating k upward and testing
 *            whether fractional part of k*log10(2) lies in [log10(4),
 *            log10(5))
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, leading_digit_is_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------  required headers  ---------- */
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/leading_digit_is_4.h"

/* ----------  cache for sequence values  ---------- */
static seq_t *cache       = NULL;   /* pointer to allocated array of seq_t */
static size_t size       = 0;       /* number of terms stored in cache    */
static size_t capacity   = 0;       /* allocated array capacity           */
static idx_t last_k_checked = 0;    /* last k that was tested             */

/* ensure that cache has space for at least new_capacity entries */
static void ensure_cache_capacity(size_t new_capacity)
{
    if (new_capacity <= capacity) return;

    size_t new_cap = (capacity == 0 ? 64 : capacity << 1);
    while (new_cap < new_capacity) new_cap <<= 1;

    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache) abort();         /* allocation failure */

    /* copy existing elements (seq_t may be non‑trivial, use assignment) */
    for (size_t i = 0; i < size; ++i) {
        new_cache[i] = cache[i];
    }

    /* free old array if it existed */
    if (cache) free_seq_array(cache);

    cache     = new_cache;
    capacity  = new_cap;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    /* extend cache until at least n terms are available */
    while (size < (size_t)n) {
        /* test the next k */
        ++last_k_checked;
        if (leading_digit_is_4(last_k_checked)) {
            ensure_cache_capacity(size + 1);
            cache[size++] = (seq_t)last_k_checked;
        }
    }

    return cache[n - 1];           /* zero‑based indexing in cache */
}
