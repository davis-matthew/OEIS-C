/*
 * A255527  Where records occur in A255437.
 * https://oeis.org/A255527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A255527: Where records occur in A255437.
   From the OEIS pattern we find that if k is the smallest integer
   satisfying  k² + 3k + 3  ≥  n   (with n ≥ 1), then
   a(n) = n + k(k+3)/2 .
   The following implementation follows this formula.
*/
seq_t a(idx_t n)
{
    /* n ≥ 1; for safety return 0 for n == 0 */
    if (n == 0)
        return 0;

    /* find the block index k (k starts from 0) */
    idx_t k = 0;
    unsigned __int128 C = 3;                   /* C_k = k^2 + 3k + 3 */
    while ((unsigned __int128)n > C) {
        ++k;
        C = (unsigned __int128)k * k + (unsigned __int128)3 * k + 3;
    }

    /* compute k*(k+3)/2 with seq_t arithmetic */
    seq_t kseq = (seq_t)k;
    seq_t diff = (kseq * (kseq + 3)) / 2;

    return (seq_t)n + diff;
}
