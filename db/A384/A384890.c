/*
 * A384890  Number of maximal anti-runs (increasing by more than 1) in the
 *          binary indices of n.
 * https://oeis.org/A384890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS A384890: Number of maximal anti‑runs (increasing by more than 1) 
 * in the binary indices of n.
 *
 * Binary indices of n are the positions of 1 bits in the reversed binary expansion 
 * (i.e., positions of set bits starting from 1).  The maximal anti‑runs are the
 * maximal contiguous groups of these indices where each successive difference
 * is greater than 1.  Equivalently, for n>0 the number of anti‑runs equals
 * 1 plus the number of adjacent set-bit positions whose difference is exactly 1.
 * For n==0 there are no indices, so a(0)=0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* no set bits => no anti‑runs */
        return 0;

    seq_t result = 1;      /* at least one anti‑run for every non‑zero n   */
    idx_t prev_pos = 0;    /* previous set‑bit position (1‑based)       */
    idx_t pos = 1;         /* current bit position (1‑based)            */
    idx_t temp = n;        /* local copy to preserve the argument       */

    while (temp) {
        if (temp & 1) {    /* bit at current position is set         */
            if (prev_pos && pos - prev_pos == 1)
                result++;  /* a gap of size 1 starts a new anti‑run */
            prev_pos = pos;
        }
        temp >>= 1;        /* move to next bit                        */
        pos++;
    }

    return result;
}
