/*
 * A268235  a(n) = Sum_{k=1..n} floor(n/k)*2^(k-1).
 * https://oeis.org/A268235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * 2^(k-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A268235. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t pow2 = 1; /* 2^(k-1) for the current k (starts with k=1, so 2^0 = 1) */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t q = n / k;           /* floor(n/k) */
        total += pow2 * (seq_t)q;  /* add floor(n/k)*2^(k-1) to total */
        pow2 *= 2;                 /* prepare 2^k for next iteration (k+1) */
    }
    return total;
}
