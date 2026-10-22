/*
 * A379215  Number of minimal edge cuts in the complete bipartite graph
 *          K_{n,n}.
 * https://oeis.org/A379215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ( n + (2^(n-1) - 1)^2 ) for n > 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_ex(idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= (seq_t)2;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;   /* a(1) = 1 */
    }

    seq_t two_power = pow2_ex(n - (idx_t)1);      /* 2^(n-1) */
    seq_t t = two_power - (seq_t)1;               /* 2^(n-1) - 1 */
    seq_t sq = t * t;                             /* (2^(n-1)-1)^2 */
    seq_t res = (seq_t)2 * ((seq_t)n + sq);       /* final result */
    return res;
}
