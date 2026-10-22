/*
 * A372304  Binomial transform of the Gray code sequence.
 * https://oeis.org/A372304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * A003188(k) where A003188(k)
 *            = k XOR (k >> 1) (Gray code of k)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;      /* accumulator for the result */
    seq_t bin = 1;      /* binomial coefficient C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* Gray code of k */
        seq_t gray = ((seq_t)k) ^ (((seq_t)k) >> 1);

        /* add current term */
        sum += bin * gray;

        /* update binomial coefficient for next k: C(n, k+1) = C(n,k)*(n-k)/(k+1) */
        if (k != n)
            bin = (bin * ((seq_t)(n - k))) / ((seq_t)(k + 1));
    }
    return sum;
}
