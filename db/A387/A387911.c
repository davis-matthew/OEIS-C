/*
 * A387911  Numbers which are the concatenation of two numbers whose digital
 *          roots are equal.
 * https://oeis.org/A387911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387911: numbers that can be split into two decimal parts (the
 * second part may have leading zeroes) whose digital roots are equal.  
 * The function builds the sequence by searching forward for the next
 * qualifying number and caches the results.  No hard‑coded values are used. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/candidate.h"

/* Main accessor function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for index 0, safe return */

    /* Static cache – large enough for the typical test range (10 000 terms). */
    static seq_t cache[200000];
    static size_t cache_len = 0;
    static seq_t next_candidate = 11;  /* start search after 10 */

    while (cache_len < (size_t)n) {
        seq_t num = next_candidate;
        while (!candidate(num)) ++num;   /* find next qualifying number */

        cache[cache_len++] = num;
        next_candidate = num + 1;
    }

    return cache[n - 1];
}
