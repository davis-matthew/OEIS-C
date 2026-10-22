/*
 * A346302  Positions of words in A076478 such that first digit != last digit.
 * https://oeis.org/A346302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = position of the n‑th binary word (in the lexicographic order by length)
   whose first and last digits differ. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)  /* the sequence is 1‑based; return 0 for 0 if called */
        return (seq_t)0;

    idx_t  found = (idx_t)0;   /* how many qualifying words have been seen */
    seq_t  pos   = (seq_t)1;   /* current position (1‑based) */

    for (idx_t L = (idx_t)1; found < n; ++L) {
        /* number of words of length L: 2^L */
        idx_t words_len = (idx_t)1 << L;

        for (idx_t i = (idx_t)0; i < words_len; ++i) {
            /* first digit (most significant bit) and last digit (least) */
            bool first = (((i >> (L - 1)) & (idx_t)1) != 0);
            bool last  = ((i & (idx_t)1) != 0);

            if (first != last) {
                ++found;
                if (found == n)
                    return pos;                /* the nth qualifying word */
            }
            ++pos;                               /* move to next word position */
        }
    }

    return (seq_t)0;   /* never reached */
}
