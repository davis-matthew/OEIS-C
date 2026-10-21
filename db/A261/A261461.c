/*
 * A261461  a(n) is the smallest nonzero number that is not a substring of n
 *          in its binary representation.
 * https://oeis.org/A261461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest non-zero integer not appearing as a contiguous binary substring
   of the binary representation of n.  (OEIS A261461) */
#include <string.h>   /* for memset */

seq_t a(idx_t n)
{
    /* Determine the bit length of n.  For n==0 we treat it as length 1. */
    idx_t L;
    if (n == 0) {
        L = 1;
    } else {
        L = 0;
        idx_t tmp = n;
        while (tmp) {
            ++L;
            tmp >>= 1;
        }
    }

    /* We only need to remember substrings up to a safe fixed limit.
       For n up to 10000 this limit of 65536 is more than enough,
       as the number of possible substrings is <= L*(L+1)/2 < 2100. */
    static unsigned char seen[65536];
    memset(seen, 0, sizeof(seen));

    /* Generate all contiguous binary substrings of n and mark them. */
    for (idx_t i = 0; i < L; ++i) {
        idx_t val = 0;
        for (idx_t j = i; j < L; ++j) {
            int shift = (int)(L - 1 - j);
            int bit   = (int)((n >> shift) & 1);
            val = (val << 1) | (idx_t)bit;
            if (val < 65536) {
                seen[(idx_t)val] = 1;
            }
        }
    }

    /* Find the smallest positive integer not marked. */
    seq_t k = 1;
    while ((idx_t)k < 65536 && seen[(idx_t)k]) {
        ++k;
    }
    /* The above loop is guaranteed to terminate for the range of n
       considered by the tests, because the number of substrings
       is far smaller than 65536. */
    return k;
}
