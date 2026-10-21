/*
 * A250771  Number of (3+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*2^(n-1) + n^2 + 11*n + 8
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence; behaviour for n<1 is irrelevant. */
    if (n == 0)
        return 0;                    /* undefined for n<1, harmless fallback */

    /* Compute 2^(n-1) iteratively. */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow2 *= (seq_t)2;           /* multiply by 2 each step */

    seq_t n_seq = (seq_t)n;          /* promote the index to seq_t */

    seq_t result = pow2 * (seq_t)16; /* 16*2^(n-1) */

    result += n_seq * n_seq;         /* n^2 */
    result += (seq_t)11 * n_seq;     /* 11*n */
    result += (seq_t)8;              /* +8 */

    return result;
}
