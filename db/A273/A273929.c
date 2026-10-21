/*
 * A273929  Numbers that are congruent to {5, 6, 7} mod 8 and are squarefree.
 * https://oeis.org/A273929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are congruent to {5, 6, 7} mod 8 and are squarefree.
   This function returns the nth such number (offset 1). */
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t cand = (seq_t)5;          /* first candidate with 5 mod 8 */
    idx_t  count = 0;               /* how many have been found so far */

    while (1) {
        if (is_squarefree(cand)) {
            ++count;
            if (count == n)       /* we have found the requested term */
                return cand;
        }
        ++cand;                     /* try next integer */
        /* Skip numbers that are not 5, 6 or 7 mod 8 */
        while ((cand & (seq_t)7) <= (seq_t)4)
            ++cand;
    }
}
