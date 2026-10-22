/*
 * A327704  The minimal size of a partition lambda of n such that every
 *          partition of n with at most 4 parts can be obtained by coalescing
 *          the parts of lambda.
 * https://oeis.org/A327704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = 1 + a( floor(3*n/4) ) for n ≥ 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (minimal size of a partition λ of n such that every partition of n
   with at most 4 parts can be obtained by coalescing the parts of λ) */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t cur = n;
    while (cur > 0) {
        ++res;
        cur -= (cur + 3) / 4;          /* floor(3*cur/4) = cur - ceil(cur/4) = cur - (cur+3)/4 */
    }
    return res;
}
