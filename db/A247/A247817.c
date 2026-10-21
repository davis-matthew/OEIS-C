/*
 * A247817  a(n) = Sum_{k=2..n} 4^k.
 * https://oeis.org/A247817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} 4^k (offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Return the n-th term of sequence A247817.
        The sequence is defined for n ≥ 1 with a(1)=0.
        For n=1 the sum is empty; for n≥2 we iterate over the
        powers of four.  The implementation uses only integer
        arithmetic and works for any seq_t (fixed‑size or
        arbitrary‑precision).  It is O(n) and fast enough for
        n up to 10 000. */
    if (n < 2)
        return (seq_t)0;          /* a(1) = 0, a(0) undefined but harmless */

    seq_t sum = (seq_t)0;
    seq_t pow = (seq_t)16;        /* 4^2, first term of the sum */

    for (idx_t k = 2; k <= n; ++k) {
        sum += pow;
        pow *= (seq_t)4;          /* prepare next term 4^(k+1) */
    }
    return sum;
}
