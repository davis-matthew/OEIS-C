/*
 * A276091  Numbers obtained by reinterpreting base-2 representation of n in
 *          A001563-base (A276326): a(n) = Sum_{k>=0}
 *          A030308(n,k)*A001563(k+1).
 * https://oeis.org/A276091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k≥0} A030308(n,k) * A001563(k+1) where A001563(m) = m
 *            * m! (so a(n) = Σ_{k} bit_k(n)*(k+1)*(k+1)! )
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 2047, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result    = 0;
    seq_t factorial = 1;          /* will hold (k+1)! for current bit k */
    idx_t k         = 0;
    idx_t temp      = n;

    while (temp)
    {
        factorial = factorial * (seq_t)(k + 1);     /* update to (k+1)! */
        if (temp & 1)
            result = result + factorial * (seq_t)(k + 1);
        temp >>= 1;
        k++;
    }
    return result;
}
