/*
 * A328783  Numbers k such that k and k^2 contain at least one zero but not
 *          the same number of 0's.
 * https://oeis.org/A328783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate integers k; a(n) is the n‑th integer for which
   k and k² both have at least one zero digit in decimal, but the number
   of zeros in k differs from that in k². */

#include <stdbool.h>
#include <stddef.h>       /* size_t */
#include "../utils/digit_counts.h"
#include "../utils/append_candidate.h"

static bool k_has_property(seq_t k)
{
    seq_t k2;
    unsigned char cnt[10], cnt2[10];

    /* Count digits of k and k² */
    digit_counts(k, cnt);
    k2 = k * k;
    digit_counts(k2, cnt2);

    /* Must contain at least one zero, and counts differ */
    return cnt[0] && cnt2[0] && cnt[0] != cnt2[0];
}

seq_t a(idx_t n)
{
    /* Static cache of found values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Keep track of the next integer to test */
    static idx_t current = 0;   /* last tested integer (0 means none) */

    /* Generate terms until we have at least n of them */
    while (len < n) {
        ++current;                           /* next candidate */
        seq_t k = (seq_t)current;
        if (k_has_property(k))
            append_candidate(&cache, &len, &cap, k);
    }

    return cache[n - 1];
}
