/*
 * A384154  a(n) is the number of binary strings of length n whose shortest
 *          run of 1s is of length 2.
 * https://oeis.org/A384154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   count_min_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_min_run.h"
/* Computes a(n) = number of binary strings of length n whose
   shortest run of 1s is of length 2.
   Implementation counts strings where all runs of 1s have length >=2
   (call this S2) and strings where all runs have length >=3
   (call this S3).  Then a(n)=S2-S3.  The counts are obtained by a
   simple DP that keeps track of the current run length of 1s. */

seq_t a(idx_t n) {
    seq_t g2 = count_min_run(n, 2);
    seq_t g3 = count_min_run(n, 3);
    return g2 - g3;
}
