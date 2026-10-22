/*
 * A357708  Numbers k such that the k-th composition in standard order has sum
 *          equal to twice its maximum part.
 * https://oeis.org/A357708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357708: returns the n-th integer k whose k-th
   composition in standard order has sum equal to twice its maximum part.
   The value is found by enumerating k and testing the composition property. */

#include <stdint.h>

static inline int is_valid(idx_t k)
{
    idx_t temp = k;
    idx_t bitlen = 0;          /* number of bits in k */
    idx_t last = 0;            /* position of last set bit, start with 0 */
    idx_t maxgap = 0;          /* maximum difference between consecutive set bits */
    idx_t pos = 1;             /* current bit position (1‑based) */

    while (temp) {
        if (temp & 1u) {        /* bit at this position is set */
            idx_t gap = pos - last;
            if (gap > maxgap) maxgap = gap;
            last = pos;
        }
        temp >>= 1;
        pos++;
        bitlen++;
    }

    if (bitlen & 1u) return 0;   /* L must be even */
    idx_t half = bitlen >> 1;    /* t = L/2 */
    return maxgap == half;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (is_valid(k)) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
}
