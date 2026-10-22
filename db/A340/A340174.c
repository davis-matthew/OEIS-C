/*
 * A340174  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 5-point
 *          set but are incident to the same v...
 * https://oeis.org/A340174
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340174: a(n) = 7*31^(n-1) - 23*15^(n-1) + 4*7^n - 5*3^n + 3,  a(3)=2792 */

seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* below the offset, value is undefined */

    seq_t pow31 = pow_seq((seq_t)31, n - 1);
    seq_t pow15 = pow_seq((seq_t)15, n - 1);
    seq_t pow7  = pow_seq((seq_t)7 , n);
    seq_t pow3  = pow_seq((seq_t)3 , n);

    seq_t result = (seq_t)7 * pow31;
    result += (seq_t)4 * pow7;
    result += (seq_t)3;
    result -= (seq_t)23 * pow15;
    result -= (seq_t)5  * pow3;

    return result;
}
