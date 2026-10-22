/*
 * A386920  a(n) = Sum_{k=0..n} binomial(4*n,k) * binomial(3*n-k,n-k).
 * https://oeis.org/A386920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n,k) * binomial(3*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t N = n;
    seq_t sum = 0;
    seq_t B1 = 1;               /* C(4N,0) */
    /* compute C(3N,N) multiplicatively: C(3N,N) = Prod_{i=1..N} (2N + i)/i */
    seq_t B2 = 1;
    for (idx_t i = 1; i <= N; ++i)
        B2 = B2 * ((seq_t)(2 * N + i)) / ((seq_t)i);

    for (idx_t k = 0; k <= N; ++k)
    {
        sum += B1 * B2;
        if (k == N)
            break; /* avoid division by zero for n=0 */

        /* update binomials for next k */
        B1 = B1 * ((seq_t)(4 * N - k)) / ((seq_t)(k + 1));
        B2 = B2 * ((seq_t)(N - k)) / ((seq_t)(3 * N - k));
    }
    return sum;
}
