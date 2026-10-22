/*
 * A372665  a(n) = Sum_{j=1..n} Sum_{k=1..n} phi(2*j*k) / phi(j*k).
 * https://oeis.org/A372665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 - ((n+1)/2)^2
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t to avoid overflow on intermediate results */
    seq_t sn   = (seq_t)n;
    seq_t nSq  = sn * sn;
    seq_t oddCnt = ((seq_t)n + (seq_t)1) / (seq_t)2;
    seq_t oddSq = oddCnt * oddCnt;
    return (seq_t)2 * nSq - oddSq;
}
