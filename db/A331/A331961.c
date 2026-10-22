/*
 * A331961  a(n) is the greatest square number k such that n AND k = k (where
 *          AND denotes the bitwise AND operator).
 * https://oeis.org/A331961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{ k : k is a perfect square and (n & k) == k }
 * Range:     u8 n <= 70, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t best  = 0;

    /* iterate over square numbers up to n */
    for (idx_t i = 1; ; ++i)
    {
        seq_t sq = (seq_t)i * (seq_t)i;
        if (sq > n_seq)          /* squares grow monotonically */
            break;
        if ((n_seq & sq) == sq)  /* sq is a subset of bits of n */
            best = sq;
    }
    return best;
}
