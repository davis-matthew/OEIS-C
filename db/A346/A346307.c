/*
 * A346307  Number of runs in the n-th word in A076478.
 * https://oeis.org/A346307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of runs in the n‑th binary word of A076478
   (words of length 1,2,3,… in lexicographic order).
   Let l = ⌊log₂(n+1)⌋  (the word length),
   offset = n – (2ᶫ – 1)  (its position in the block of length l).
   Then a(n) = 1 + popcount( (offset XOR (offset>>1)) & ((1<<(l-1))-1) ). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* l = floor(log2(n+1)) */
    idx_t t = n + 1;
    idx_t l = 0;
    while (t >> 1) { t >>= 1; ++l; }

    /* offset of the word within its length block */
    idx_t offset = n - (((idx_t)1 << l) - 1);

    /* transitions between adjacent bits of the l‑bit word */
    idx_t diff  = offset ^ (offset >> 1);
    idx_t mask  = (( (idx_t)1 << (l - 1) ) - 1);
    unsigned long long bits = (unsigned long long)(diff & mask);
    unsigned int transitions = __builtin_popcountll(bits);

    return (seq_t)(transitions + 1);
}
