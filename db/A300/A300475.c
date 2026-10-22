/*
 * A300475  a(n) is the least positive k such that the binary representation n
 *          appears in front of the binary representation of 1/k (ignoring the
 *          radix point and the leading zeros and adding trailing zeros if
 *          necessary in case of a terminating expansion).
 * https://oeis.org/A300475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k>0 such that ∃i≥0 with floor(2^i / k) = n.
   This definition is equivalent to the OEIS specification of A300475.
   The algorithm simply tests successive powers of two, computing
   k = ⌊2^i / n⌋ and checking whether floor(2^i / k) equals n.
   The first i giving a match yields the minimal k, and this
   i is always found in at most about log₂(2n) iterations. */

#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* unused, sequence is 1‑based */
    seq_t n_seq = (seq_t)n;        /* promote to seq_t once          */
    for (idx_t i = 0; ; ++i) {
        seq_t p = pow2(i);         /* p = 2^i, safe for arbitrary size */
        seq_t k = p / n_seq;       /* floor division, k may be 0        */
        if (k == 0) continue;      /* n > 2^i, try next power           */
        if ((p / k) == n_seq) {
            return k;              /* first matching k is minimal       */
        }
    }
}
