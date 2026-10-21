/*
 * A261349  T(n,k) is the decimal equivalent of a code for k that maximizes
 *          the sum of the Hamming distances between (cyclical) adjacent code
 *          words; triangle T(n,k), n>=0, 0<=k<=2^n-1, read by rows.
 * https://oeis.org/A261349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 9476, u32 n <= 9476, u64 n <= 9476,
 *            u128 n <= 9476, big any n
 * Helpers:   my_floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261349: Anti‑Gray code
   a(n) is the row‑by‑row linearization of the triangle T(r,k) where
   r = floor(log2(n+1)) and k = n – (2^r – 1).  The triangle is defined by

       T(r,k) = gray(k/2)                         , k even
              = 2^r – 1 – gray((k-1)/2)          , k odd

   where gray(x) = x XOR (x >> 1) is the binary reflected Gray code.
*/

#include <stdint.h>
#include "../utils/my_floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* row number r = floor(log2(n+1)) */
    idx_t r = my_floor_log2_uint64(n + (idx_t)1);

    /* offset within row: k = n - (2^r - 1) */
    idx_t offset = n - (((idx_t)1 << r) - 1);

    /* gray code of m = floor(k/2) */
    idx_t m = offset >> 1;
    seq_t gray = (seq_t)m ^ ((seq_t)m >> 1);

    /* 2^r as seq_t */
    seq_t pow2_r = (seq_t)((idx_t)1 << r);

    if (offset & 1U) {            /* k is odd */
        return pow2_r - (seq_t)1 - gray;
    } else {                      /* k is even */
        return gray;
    }
}
