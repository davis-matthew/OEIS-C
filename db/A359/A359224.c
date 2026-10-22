/*
 * A359224  Numbers whose decimal representation is the reverse of their
 *          base-7 representation.
 * https://oeis.org/A359224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by testing each integer until n terms have been found. */
/*  A351224: Numbers whose decimal representation is the reverse of
 *  their base‑7 representation.  n is 1‑based (a(1)=0). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

/* Check if x satisfies the reverse‑base‑7 property. */
static bool is_special(seq_t x)
{
    if (x == 0)                /* 0 is a trivial solution */
        return true;

    /* Convert x to base 7, least‑significant digit first. */
    unsigned char digits[64];
    size_t t = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[t++] = (unsigned char)(tmp % 7);
        tmp /= 7;
    }

    /* Find first non‑zero digit when the base‑7 digits are read in
     * reverse order (i.e., from most‑significant to least‑significant). */
    size_t i = 0;
    while (i < t && digits[i] == 0)
        i++;

    /* Build the decimal number from the reversed digits, skipping
     * any leading zeros that may result from the reversal. */
    seq_t val = 0;
    for (; i < t; ++i)
        val = val * 10 + digits[i];

    return val == x;
}

/* External entry point: returns the n‑th term of the sequence
 * (offset 1: a(1)=0). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t cap   = 0;      /* allocated capacity of cache   */
    static size_t len   = 0;      /* number of terms stored       */
    static seq_t last   = 0;      /* last number examined         */

    if (n == 0)                /* offset is 1; undefined for n==0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        if (is_special(last)) {
            /* Grow the cache if necessary. */
            if (len == cap)
                grow_seq(&cache, &cap, len + 1);
            cache[len++] = last;
        }
        ++last;
    }

    return cache[n - 1];
}
