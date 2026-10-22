/*
 * A301336  a(n) = total number of 1's minus total number of 0's in binary
 *          expansions of 0, ..., n.
 * https://oeis.org/A301336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = 2 * Σ_{k=0..n} popcount(k) - Σ_{k=0..n} bitlen(k) where
 *            bitlen(0)=1 and bitlen(k)=⌊log₂(k)⌋+1 for k>0
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A301336: total number of 1's minus total number of 0's
   in binary expansions of 0,1,...,n. */
seq_t a(idx_t n)
{
    seq_t total_ones = 0;
    seq_t total_len  = 1;                 /* length of the number 0 */

    /* Sum of popcounts */
    for (idx_t i = 0; ; ++i)
    {
        idx_t bit = (idx_t)1 << i;       /* 2^i   */
        if (bit > n) break;

        if (i == 63)                     /* avoid overflow of 2^(i+1) */
        {
            if (n >= bit)
                total_ones += (seq_t)(n - bit + 1);
            continue;
        }

        idx_t cycle   = bit << 1;           /* 2^(i+1) */
        idx_t full    = (n + 1) / cycle;    /* full cycles */
        seq_t ones    = (seq_t)full * bit;  /* ones in full cycles */
        idx_t rem     = (n + 1) % cycle;    /* remainder of the last cycle */
        if (rem > bit)                     /* additional ones in the remainder */
            ones += (seq_t)(rem - bit);

        total_ones += ones;
    }

    /* Sum of bit lengths */
    for (idx_t i = 0; ; ++i)
    {
        idx_t bit = (idx_t)1 << i;
        if (bit > n) break;

        /* All numbers ≥ bit contribute one more bit to their length */
        total_len += (seq_t)(n - bit + 1);
    }

    return total_ones * 2 - total_len;
}
