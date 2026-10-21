/*
 * A285054  Numbers whose sum of digits are congruent (mod 10) to the string
 *          1,2, ..., 9.
 * https://oeis.org/A285054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (floor((n-1)/9)+1)-th smallest positive integer with
 *            digit sum congruent to ((n-1)%9)+1 (mod 10). This follows the
 *            definition of sequence A285054
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   digit_sum_mod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum_mod10.h"

/* Static cache of already computed terms, plus next candidate per residue. */
static seq_t *cache = NULL;
static size_t cache_len = 0;          /* number of items stored (>= n needed) */
static seq_t next_val[10];            /* next_value[1..9] for each residue */

/* Initialise static structures on first call. */
static void init_sequence(void) {
    if (cache == NULL) {
        cache_len = 0;
        /* Smallest positive integer with sum%10 = r is r itself (1..9). */
        for (int r = 1; r <= 9; ++r) {
            next_val[r] = (seq_t)r;
        }
    }
}

/* Main function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) {           /* safety for out-of-range */
        return (seq_t)0;
    }
    init_sequence();

    /* Extend cache up to position n. */
    while (cache_len < (size_t)n) {
        /* Determine residue for this index. */
        int r = (int)(((cache_len) % 9) + 1);  /* cache_len is 0‑based */

        /* Fetch the current candidate for this residue. */
        seq_t val = next_val[r];

        /* Store it into the cache. */
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        }
        cache[cache_len] = val;
        ++cache_len;

        /* Update the next candidate for this residue. */
        seq_t cand = val + 1;
        while (digit_sum_mod10(cand) != (unsigned)r) {
            cand += 1;
        }
        next_val[r] = cand;
    }

    return cache[(size_t)n - 1];
}
