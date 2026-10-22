/*
 * A320038  Write n in binary, then modify each run of 0's by prepending one
 *          1, and modify each run of 1's by prepending one 0. a(n) is the
 *          decimal equivalent of the result.
 * https://oeis.org/A320038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A320038
   a(n) is obtained by writing n in binary, then for each maximal run of
   equal bits prepend the opposite bit.  The resulting bit string is
   interpreted as a binary number.
   This is equivalent to a(n) = floor(A175046(n)/2). */
#include <stdint.h>

/* Computes A320038 for a given 1‑based index n. */
seq_t a(idx_t n)
{
    /* n >= 1 for the scope of the sequence.  Handle n==0 defensively. */
    if (n == 0)
        return 0;

    /* Determine the number of bits in n (without leading zeros). */
    int bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++bitlen;
    }

    seq_t res = 0;
    int prev_bit = -1;                 /* no previous bit yet */

    /* Process bits from most significant to least. */
    for (int i = bitlen - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ULL);   /* Extract bit i (0 or 1) */

        if (i == bitlen - 1) {             /* first bit: start first run */
            int opp = 1 - bit;
            res = (res << 1) | opp;       /* prepend opposite bit */
            res = (res << 1) | bit;       /* output the first bit of run */
            prev_bit = bit;
        } else if (bit == prev_bit) {      /* continuation of current run */
            res = (res << 1) | bit;
        } else {                           /* new run starts here */
            int opp = 1 - bit;
            res = (res << 1) | opp;       /* prepend opposite bit */
            res = (res << 1) | bit;       /* output the first bit of new run */
            prev_bit = bit;
        }
    }

    return res;
}
