/*
 * A340555  T(n, k) = [x^k] (2^n-1)*2^(-n-1)*((x+1)^(2^n) - (x-1)^(2^n)).
 *          Irregular triangle read by rows, for n >= 0 and 0 <= k <= 2^n.
 * https://oeis.org/A340555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 47, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A340555:
   T(r,k) = coefficient of x^k in
        (2^r-1)*2^(-(r+1))*((x+1)^(2^r) - (x-1)^(2^r))
   For r ≥ 1, T(r,k)=0 when k is even, otherwise
        T(r,k) = (2^r-1) * C(2^r , k) / 2^r.
   Row 0 has a single entry T(0,0)=0.
   a(n) is the sequence obtained by concatenating the rows
   (row 0, row 1, row 2, …).  */
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* locate the row r containing index n */
    for (idx_t r = 0; ; ++r) {
        idx_t start, len;
        if (r == 0) {
            start = 0;
            len   = 1;                 /* row 0 has one entry */
        } else {
            start = ((idx_t)1 << r) + r - 2;  /* start index of row r */
            len   = ((idx_t)1 << r) + 1;      /* row r has 2^r+1 entries */
        }
        if (n < start + len) {          /* found correct row */
            idx_t k = n - start;        /* column index within the row */

            /* Even k gives zero */
            if ((k & 1u) == 0)
                return (seq_t)0;

            /* For odd k compute the formula */
            idx_t twopow = ((idx_t)1 << r);   /* 2^r (idx_t) */
            seq_t binom_val = binom(twopow, k);
            seq_t numerator = ((seq_t)(twopow - 1)) * binom_val;
            return numerator / ((seq_t)twopow);
        }
    }
}
