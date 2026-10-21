/*
 * A285960  {01->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285960
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

/* a(n) = nth term of the word obtained by replacing every occurrence of
   "01" in the Thue–Morse sequence by a single "1".  This is the sequence
   A285960.  The Thue–Morse bit at index i (1‑based) equals the parity
   of (i-1).  The transform scans the original word, emits either the
   current bit or a "1" when a "01" pair is found, and advances past the
   consumed symbols. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* i iterates over the original Thue–Morse word (1‑based). */
    idx_t i   = 1;
    /* out counts the number of output symbols produced. */
    idx_t out = 0;

    while (1) {
        /* Thue–Morse bit at position i: parity of (i-1). */
        seq_t cur = (seq_t)__builtin_parityll((unsigned long long)(i - 1));

        if (cur == (seq_t)0) {
            /* Check whether the next bit forms the pattern 01. */
            seq_t nxt = (seq_t)__builtin_parityll((unsigned long long)i);
            if (nxt == (seq_t)1) {
                /* Replace the pair 01 by a single 1. */
                ++out;
                if (out == n)
                    return (seq_t)1;
                i += 2;          /* skip both bits of the pair */
                continue;
            }
        }

        /* Emit the current bit (which is not the start of a 01). */
        ++out;
        if (out == n)
            return cur;
        ++i;  /* advance to the next original position */
    }
}
