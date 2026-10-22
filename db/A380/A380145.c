/*
 * A380145  The binary expansion of a(n) is an initial 1 bit then tracks where
 *          the swaps occur in the exchange sort algorithm sorting the binary
 *          expansion of n into decreasing order.
 * https://oeis.org/A380145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is obtained by simulating exchange sort on the binary
   expansion of n into decreasing order. 
   The binary representation of a(n) starts with a leading 1 (fixed)
   and for every compare–swap operation in the algorithm a 1 bit is
   appended, otherwise a 0 bit.  The compare‑swap is performed for all
   pairs (i,j) with 1 ≤ i < j ≤ L, where L is the length of n’s binary
   expansion.  The vector B is mutated on each swap. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, but safe guard */

    /* Determine the number of bits (length L) of n */
    int L = 0;
    uint64_t tmp = n;
    while (tmp) {                  /* n ≥ 1, so at least one iteration */
        ++L;
        tmp >>= 1;
    }

    /* Extract the bits of n into an array, most‑significant first */
    uint8_t bits[64];
    for (int i = 0; i < L; ++i) {
        bits[i] = (n >> (L - 1 - i)) & 1ULL;
    }

    /* Build the result by simulating exchange sort */
    seq_t res = 1;                 /* leading 1 */
    for (int i = 0; i < L - 1; ++i) {
        for (int j = i + 1; j < L; ++j) {
            uint8_t swap = 0;
            if (bits[i] < bits[j]) {   /* swap condition */
                uint8_t t = bits[i];
                bits[i] = bits[j];
                bits[j] = t;
                swap = 1;
            }
            res = (res << 1) | swap;   /* append the bit */
        }
    }

    return res;
}
