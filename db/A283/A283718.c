/*
 * A283718  Numbers m such that sum of digits of 27*m is 27.
 * https://oeis.org/A283718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1286, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search of m such that sum of digits of 27*m is 27 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Handle zero index safely (offset starts at 1) */
    if (n == 0)
        return 0;

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;
    static seq_t  next_candidate = 0;

    /* Initialise cache on first call */
    if (cap == 0) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        /* Advance to next integer candidate */
        next_candidate++;
        seq_t prod = next_candidate * (seq_t)27;      /* 27 * m */

        /* Test the digit‑sum condition */
        if (digit_sum(prod) == 27) {
            /* Grow cache if necessary */
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = next_candidate;
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
