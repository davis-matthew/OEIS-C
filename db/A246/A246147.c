/*
 * A246147  Index sequence for limit-block extending A010060 (Thue-Morse
 *          sequence) with first term as initial block.
 * https://oeis.org/A246147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   thue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:  a(n) is the minimal index m(i) at which the block
   of length i + 1 in the Thue–Morse sequence starts and extends the
   previous block of length i by one term on the right.
   The algorithm searches for the next such block iteratively. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/thue.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t pat_size = (size_t)(n + 1);
    unsigned char *pat = (unsigned char *)malloc(pat_size * sizeof(unsigned char));
    if (!pat)
        return (seq_t)0;   /* allocation failed */

    pat[0] = 0;          /* Thue–Morse at position 0 is 0 */
    idx_t start = 0;     /* current block start (m(i)) */
    idx_t len   = 1;     /* current block length (i + 1) */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = start + 1;          /* candidate start for next block */

        /* Find the first j > start whose first len bits match pat[0..len-1] */
        while (1) {
            unsigned char ok = 1;
            for (idx_t t = 0; t < len; ++t) {
                if (thue(j + t) != pat[t]) {
                    ok = 0;
                    break;
                }
            }
            if (ok)
                break;
            ++j;
        }

        start = j;                      /* new block start */
        pat[len] = thue(start + len);   /* append the new symbol */
        ++len;                          /* block length increases by 1 */
    }

    free(pat);
    return (seq_t)start;
}
