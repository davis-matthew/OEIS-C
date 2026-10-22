/*
 * A394091  Greatest partial alternating sum of the n-th composition in
 *          standard order (row n of A066099). Row-maxima of A391983.
 * https://oeis.org/A394091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = maximum partial alternating sum of the n‑th composition in
   graded reverse‑lexicographic order (row n of A066099). The composition
   of an integer m is represented by a binary pattern of length m‑1 that
   indicates where separators occur. The function interprets the binary
   pattern corresponding to the offset of n within the block of all
   compositions of sum m, then computes the alternating partial sums
   recursively: S_j = -S_{j-1} + part_j, updating the running maximum. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Composition of 0 is the empty composition. */
    if (n == 0)
        return (seq_t)0;

    /* Determine the sum m of the composition that corresponds to index n.
     * The first composition of sum m starts at index 2^(m-1).  Find m by
     * counting how many powers of two fit into n. */
    idx_t m = 0;
    while (m < 64 && ((idx_t)1 << m) <= n)
        ++m;

    /* Offset of n within the block of compositions of sum m. */
    idx_t offset = n - ((idx_t)1 << (m - 1));

    /* Number of separator bits for compositions of sum m. */
    idx_t bits = m - 1;

    int64_t S = 0;    /* current partial alternating sum */
    int64_t maxS = 0; /* maximum observed partial alternating sum */
    int part = 1;     /* length of the current part */

    /* Scan separator bits from most significant to least significant
     * (i.e., from left to right in the composition). */
    for (int i = (int)bits - 1; i >= 0; --i) {
        if ((offset >> i) & (idx_t)1) { /* separator present */
            S = -S + part;
            if (S > maxS)
                maxS = S;
            part = 1;
        } else { /* continue current part */
            ++part;
        }
    }

    /* Final part after the last separator bit. */
    S = -S + part;
    if (S > maxS)
        maxS = S;

    return (seq_t)maxS;
}
