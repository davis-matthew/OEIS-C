/*
 * A270543  Number of ordered pairs (i,j) of integers with |i|, |j| <= n, |i *
 *          j| <= n, and i even.
 * https://oeis.org/A270543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2*n + 2*floor(n/2) +
 *            4*Sum_{k=1..floor(n/2)}floor(n/(2k))
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270543: Number of ordered pairs (i,j) with |i|,|j|≤n, |i*j|≤n, i even */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = (seq_t)1;                 /* constant 1 */
    term += (seq_t)2 * N;                  /* 2*n */
    seq_t half = N / (seq_t)2;             /* floor(n/2) */
    term += (seq_t)2 * half;               /* 2*floor(n/2) */
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= half; ++k)
    {
        seq_t denom = (seq_t)2 * (seq_t)k; /* 2*k */
        sum += N / denom;                 /* floor(n/(2k)) */
    }
    term += (seq_t)4 * sum;                /* 4*sum */
    return term;
}
