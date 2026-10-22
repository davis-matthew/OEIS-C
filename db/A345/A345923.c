/*
 * A345923  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum -2.
 * https://oeis.org/A345923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345923: a(n) = nth integer k such that the reverse‑alternating sum
 * of the k‑th composition in standard order equals -2.
 * Computed by iteratively testing successive k's. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    idx_t k       = 1;                    /* candidate k */
    idx_t found   = 0;                    /* how many suitable k's seen */

    while (1) {
        int64_t sum = 0;                 /* reverse‑alternating sum for this k */
        int     sign = 1;                /* +1 for first difference, then alternate */
        idx_t  lastpos = 0;              /* position of previous 1‑bit (starting at 0) */
        idx_t  pos = 1;                  /* current bit position (1‑based) */
        idx_t  tmp = k;                  /* copy for bit extraction */

        while (tmp) {
            if (tmp & 1ULL) {
                idx_t diff = pos - lastpos;                /* difference of positions */
                sum += sign * (int64_t)diff;               /* accumulate with sign */
                sign = -sign;                              /* alternate for next */
                lastpos = pos;                              /* update previous 1‑bit pos */
            }
            ++pos;
            tmp >>= 1;
        }

        if (sum == -2) {
            ++found;
            if (found == n)
                return (seq_t)k;          /* cast k to seq_t for return */
        }
        ++k;
    }
}
