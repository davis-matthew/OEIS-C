/*
 * A368181  a(1) = 1; for n > 1, a(n) is the smallest positive integer that
 *          has not yet appeared which shares no digit with the sum of all
 *          previous terms a(1)..a(n-1).
 * https://oeis.org/A368181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1882, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   digit_frequencies, has_common_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest unused positive integer that shares no decimal
   digit with the sum of all previous terms a(1)..a(n‑1).  The sequence
   terminates after 14594 terms; the largest term is 888888.  This
   implementation generates terms up to the requested index and caches
   them.  It uses a fixed boolean array that is sufficiently large to
   record every value that can appear (up to 1 000 000).  Each term is
   computed in O(1) amortised time. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Utility supplied by the build environment. */
#include "../utils/digit_frequencies.h"
#include "../utils/has_common_digit.h"

/* Determine the set of decimal digits present in a seq_t value.
   The returned mask has bit d set iff digit d appears in x. */
static inline uint16_t digit_mask_seq(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);
    uint16_t mask = 0;
    for (int d = 0; d < 10; ++d) {
        if (freq[d]) mask |= (1u << d);
    }
    return mask;
}

/* Constants: the sequence has 14594 terms, the largest term is 888888.
   An array of size 1 000 000 is enough to track all used values. */
#define MAX_TERMS   14594
#define MAX_USED    1000000   /* must be > 888888 */

/* Static arrays and state variables. */
static seq_t    seq[MAX_TERMS + 1];
static bool     used[MAX_USED];
static idx_t    seqlen   = 0;  /* number of terms currently stored */
static seq_t    sum      = 0;  /* sum of all stored terms */

/* Initialise static data the first time a() is called. */
static void init_sequence(void)
{
    memset(used, 0, sizeof(used));
    seq[0] = 1;
    used[1] = true;
    sum = seq[0];
    seqlen = 1;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 1; safe fallback */

    if (seqlen == 0) init_sequence();

    /* Generate terms until we have at least n terms. */
    while (seqlen < n) {
        uint16_t sumMask = digit_mask_seq(sum);

        /* Find the smallest unused candidate that avoids sumMask. */
        idx_t cand = 1;
        for (;;) {
            if (cand < MAX_USED && !used[cand] && !has_common_digit(cand, sumMask))
                break;
            ++cand;
        }

        seq[seqlen] = (seq_t)cand;
        used[cand] = true;
        sum += (seq_t)cand;
        ++seqlen;
    }

    /* Return the required term (indices in OEIS start at 1). */
    return seq[n - 1];
}
