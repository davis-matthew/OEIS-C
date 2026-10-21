/*
 * A283833  For t >= 0, if 2^t + t - 3 <= n <= 2^t + t - 1 then a(n) = 2^t -
 *          1, while if 2^t + t - 1 < n < 2^(t+1) + t - 3 then a(n) = 2^(t+1)
 *          + t - 2 - n.
 * https://oeis.org/A283833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 4104, u32 n <= 4104, u64 n <= 4104,
 *            u128 n <= 4104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A283833.
   For integer t ≥ 1
       if 2^t + t − 3 ≤ n ≤ 2^t + t − 1 then a(n) = 2^t − 1,
       if 2^t + t − 1 < n < 2^(t+1) + t − 3 then a(n) = 2^(t+1) + t − 2 − n,
       if n = 2^(t+1) + t − 3 then a(n) = 1.
   (Offset 0: a(0) = 1)
*/

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t n_val = (seq_t)n;
    seq_t pow2 = 1;           /* will become 2^t after the shift */

    for (idx_t t = 1;; ++t) {
        pow2 <<= 1;                      /* pow2 = 2^t */
        seq_t U      = pow2 + (seq_t)t - 1;                    /* 2^t + t − 1 */
        seq_t maxSt  = (pow2 << 1) + (seq_t)t - 3;              /* 2^(t+1) + t − 3 */

        if (n_val <= U) {                /* constant block */
            return pow2 - 1;
        }
        if (n_val < maxSt) {             /* decreasing block */
            seq_t S = (pow2 << 1) + (seq_t)t - 2; /* 2^(t+1) + t − 2 */
            return S - n_val;
        }
        if (n_val == maxSt) {            /* special value 1 */
            return 1;
        }
        /* otherwise continue to the next t */
    }
}
