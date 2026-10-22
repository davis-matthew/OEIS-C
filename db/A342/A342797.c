/*
 * A342797  Irregular triangle read by rows: T(n, k) is the k-th antidiagonal
 *          sum of the n X n matrices defined in A069480 and A078475.
 * https://oeis.org/A342797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A342797: Irregular triangle read by rows.
    Let
      r = ceil(sqrt(n)),                // row containing term n
      k = n - (r-1)^2,                  // position within the row
    then
      if (k <= r)          a(n) = k * (k*k + 1) / 2;          // A006003(k)
      else if (k == 2*r-1) a(n) = r * r;                      // = n^2
      else                 a(n) = (k*k*k + 2*r - 6*k*k*r
                                   - 4*r*r*r + k*(10*r*r - 1)) / 2;
*/
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* compute ceil(sqrt(n)) */
    seq_t sq = floor_sqrt((seq_t)n);
    idx_t r = (idx_t)sq;                     /* floor */
    if ((seq_t)(r) * r != (seq_t)n) r++;     /* adjust to ceil */

    /* position in the row */
    idx_t k = n - (r - 1) * (r - 1);

    /* compute the value according to the definition */
    if ((idx_t)k <= r) {
        seq_t kk = (seq_t)k;
        return kk * (kk * kk + (seq_t)1) / (seq_t)2;
    }
    if (k == 2 * r - 1) {
        seq_t rr = (seq_t)r;
        return rr * rr;
    }

    /* intermediate values for the general formula */
    seq_t k_seq = (seq_t)k;
    seq_t r_seq = (seq_t)r;
    seq_t k2 = k_seq * k_seq;
    seq_t r2 = r_seq * r_seq;
    seq_t r3 = r2 * r_seq;
    seq_t k3 = k2 * k_seq;

    seq_t num = k3
              + (seq_t)2 * r_seq
              - (seq_t)6 * k2 * r_seq
              - (seq_t)4 * r3
              + k_seq * ((seq_t)10 * r2 - (seq_t)1);

    return num / (seq_t)2;
}
