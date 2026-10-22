/*
 * A390595  Numbers k such that the binary length of k is the same as the
 *          binary length of 3*k+1 reduced by the largest possible power of
 *          two.
 * https://oeis.org/A390595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390595:
   Numbers k such that the binary length of k equals the binary length
   of the odd part of 3*k+1 (i.e. 3*k+1 with all trailing zeros removed). */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 for safety */

    idx_t count = 0;                      /* how many terms have been found */
    seq_t k = 0;                          /* current candidate k */

    while (count < n) {
        k = k + 1;                        /* examine next integer */

        /* compute 3*k+1 and strip all powers of two */
        seq_t r = (seq_t)3 * k + (seq_t)1;
        while ((r & (seq_t)1) == (seq_t)0) {
            r = r >> 1;
        }

        unsigned bl_k = bitlen_seq_t(k);
        unsigned bl_r = bitlen_seq_t(r);

        if (bl_k == bl_r) {
            ++count;                      /* k satisfies the condition */
        }
    }

    return k;                             /* the n‑th term */
}
