/*
 * A301297  Distance from n to nearest Catalan number.
 * https://oeis.org/A301297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{k>=0} |n - C_k|, where C_k is the k‑th Catalan
 *            number
 * Range:     u8 n <= 42, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert n to the sequence type */
    idx_t   k   = 0;
    seq_t   prev = catalan(0);    /* C_0 = 1 */

    while (1) {
        idx_t next_k = k + 1;
        seq_t curr   = catalan(next_k);

        if (curr >= sn) {         /* sn is between prev and curr inclusive */
            seq_t d1 = (sn > prev) ? (sn - prev) : (prev - sn);  /* |sn‑prev| */
            seq_t d2 = (curr > sn) ? (curr - sn) : (sn - curr);  /* |curr‑sn| */
            return (d1 < d2) ? d1 : d2;
        }

        k = next_k;
        prev = curr;
    }
}
