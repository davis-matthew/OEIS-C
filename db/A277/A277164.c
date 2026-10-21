/*
 * A277164  Lexicographically least sequence of nonnegative integers that
 *          avoids 17/10-powers.
 * https://oeis.org/A277164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically least nonnegative integer sequence
 *            avoiding a 17/10–power. The sequence is generated greedily: for
 *            each n, the smallest candidate c that does not create a suffix
 *            of length 17t whose first 10t elements equal the following 7t
 *            elements (for any t ≥ 1) is chosen
 * Range:     u8 n <= 9548, u16 n <= 9548, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *seq = NULL;
    static idx_t  len = 0;   /* number of elements already stored (seq[0..len-1]) */
    static idx_t  cap = 0;   /* capacity of the allocation */

    /* initialise cache on first call */
    if (len == 0) {
        cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until it contains a(n) */
    while (len <= n) {
        /* try integer candidates in increasing order */
        seq_t cand = 0;
        while (true) {
            /* ensure storage is large enough for this candidate */
            if (len >= cap) {
                cap *= 2;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len] = cand;            /* tentatively append candidate */

            /* check whether this creates a forbidden 17/10–power */
            bool bad = false;
            idx_t upto = len;           /* index of newly appended element */
            idx_t max_t = (upto + 1) / 17;  /* largest t that fits in the suffix */

            for (idx_t t = 1; t <= max_t; ++t) {
                idx_t start = upto - 17 * t + 1;   /* start of the 17t block */

                bool match = true;
                for (idx_t i = 0; i < 7 * t; ++i) {
                    if (seq[start + i] != seq[start + 10 * t + i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {  /* found a 17/10–power, reject candidate */
                    bad = true;
                    break;
                }
            }
            if (!bad) break;   /* candidate accepted */
            ++cand;            /* try next integer */
        }
        ++len;                 /* successfully appended a term */
    }

    return seq[n];
}
