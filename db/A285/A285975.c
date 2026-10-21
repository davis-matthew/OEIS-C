/*
 * A285975  {00->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A285975 – 00->0 transform of the Thue-Morse word.
 *
 *  This implementation generates the Thue-Morse sequence
 *  bit‑by‑bit and collapses each occurrence of "00" into a single 0.
 *  The algorithm runs in O(n) time and uses only O(1) additional
 *  memory.  It is valid for any n ≥ 1 and works for all supported
 *  integer types (seq_t, idx_t).
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 1, but return 0 safely */
        return 0;

    idx_t  i = 1;        /* index in the original Thue‑Morse word   */
    idx_t  out = 0;      /* number of output terms produced so far   */
    seq_t  last = 0;     /* last produced bit                       */

    while (out < n) {
        /* bit at position i (0‑based offset i-1) */
        seq_t bit = (__builtin_popcountll((unsigned long long)(i - 1)) & 1);

        if (bit == 0) {
            /* look ahead to check if this 0 is part of a "00" pair */
            seq_t next_bit =
                (__builtin_popcountll((unsigned long long)i) & 1);

            if (next_bit == 0) {
                /* collapse the pair: output a single 0 and skip i+1 */
                last = 0;
                ++out;
                i += 2;           /* skip the second zero */
                continue;
            }
        }

        /* otherwise output the current bit */
        last = bit;
        ++out;
        ++i;
    }

    return last;
}
