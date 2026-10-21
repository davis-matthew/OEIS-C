/*
 * A285969  {0110->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth bit of the infinite word obtained from the
 *            Thue–Morse sequence A010060 by the transformation that replaces
 *            each occurrence of the block 0110 with a single 0. This function
 *            computes a(n) by scanning the Thue‑Morse word from the start,
 *            collapsing each 0110 block, and stopping when t...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Thue–Morse bit at position p (1‑based) = parity(popcount(p‑1)). */

/* typedefs seq_t and idx_t are supplied by the build; no need to
   define them here. */

#include <stdint.h>

/* Return the Thue–Morse bit at position pos (1‑based). */ 
static inline unsigned char thue_bit(idx_t pos)
{
    /* pos-1 fits in idx_t (uint64_t).  __builtin_popcountll
       is available in GCC/Clang and returns an unsigned int. */
    return (unsigned char)((__builtin_popcountll((uint64_t)(pos - 1)) & 1u) ? 1u : 0u);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1‑based index of the current position in the Thue‑Morse word. */
    idx_t pos = 1;
    /* Number of output bits already produced. */
    idx_t out_count = 0;

    while (1)
    {
        /* Bits of the potential 0110 block starting at pos. */
        unsigned char b0 = thue_bit(pos);
        unsigned char b1 = thue_bit(pos + 1);
        unsigned char b2 = thue_bit(pos + 2);
        unsigned char b3 = thue_bit(pos + 3);

        unsigned char next_bit;
        if (b0 == 0 && b1 == 1 && b2 == 1 && b3 == 0)
        {
            /* Collapse 0110 -> 0. */
            next_bit = 0;
            pos += 4;  /* skip the whole block. */
        }
        else
        {
            /* Emit the current bit. */
            next_bit = b0;
            pos += 1;
        }

        out_count += 1;
        if (out_count == n)
            return (seq_t)next_bit;
    }
}
