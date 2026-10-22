/*
 * A337784  Smaller of two consecutive oblong numbers which are anagrams of
 *          each other.
 * https://oeis.org/A337784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smaller of two consecutive oblong numbers * that are
 *            anagrams of each other. * An oblong number is n*(n+1). The
 *            sequence encodes the first member * of each pair. * *
 *            Implementation: generate oblong numbers sequentially, * test
 *            whether the next oblong number is an anagram of the current...
 * Range:     u8 none, u16 n <= 1, u32 n <= 23, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   append_candidate, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/append_candidate.h"

/* seq_t and idx_t are provided by the build */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t    *terms   = NULL;   /* pointer to dynamic array */
    static size_t   cap      = 0;       /* current capacity */
    static size_t   len      = 0;       /* number of terms stored */
    static idx_t    last_n   = 0;       /* n last examined for a test */

    /* iterate until we have at least n terms stored */
    while (len < n) {
        idx_t i = last_n + 1;                      /* next n to test */

        /* compute consecutive oblong numbers */
        seq_t m  = (seq_t)i   * (i + 1);           /* i*(i+1)   */
        seq_t m2 = (seq_t)(i+1) * (i + 2);         /* (i+1)*(i+2) */

        /* digit frequency arrays */
        unsigned char cnt1[10], cnt2[10];
        digit_counts(m,  cnt1);
        digit_counts(m2, cnt2);

        /* check whether the two numbers are anagrams */
        bool equal = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt1[d] != cnt2[d]) { equal = false; break; }
        }

        if (equal) {
            /* store m as the next term of the sequence */
            append_candidate(&terms, &len, &cap, m);
        }

        last_n = i;   /* update last examined n */
    }

    /* the sequence is 1‑based; array indices are 0‑based */
    return terms[n - 1];
}
