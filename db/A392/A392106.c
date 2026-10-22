/*
 * A392106  Concatenation of binary words using letters 1 and 2, in numerical
 *          order.
 * https://oeis.org/A392106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the digit (1 or 2) appearing at position n in the infinite
   concatenation of all binary words over {1,2}, ordered first by
   length (1,2,…) and then lexicographically.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to an idx_t (already that type) */
    idx_t target = n;
    idx_t sum   = 0;     /* total digits processed so far       */
    idx_t pow2  = 1;     /* 2^(len-1) before len incremented   */
    idx_t len   = 0;     /* current word length                 */

    for (;;)
    {
        ++len;          /* move to next word length            */
        pow2 <<= 1;     /* now pow2 == 2^len                   */
        idx_t term   = len * pow2;      /* digits contributed by this length */

        if (sum + term > target)
        {
            /* Position falls within this block of words of length `len` */
            idx_t pos_in_len  = target - sum;         /* 0‑based position in block   */
            idx_t word_index = pos_in_len / len;      /* which word of this length   */
            idx_t offset     = pos_in_len % len;      /* 0‑based offset inside word  */
            idx_t bitpos     = (len - 1) - offset;    /* position of the desired bit in word_index */

            /* Extract that bit; 0 → 1, 1 → 2 */
            idx_t bit = (word_index >> (unsigned)bitpos) & 1;
            return (seq_t)(bit ? 2 : 1);
        }
        sum += term;     /* continue to next length              */
    }
}
